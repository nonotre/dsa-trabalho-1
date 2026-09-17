#!/usr/bin/env bash
# Mede somente a parte 3 e grava arquivos separados dos resultados do grupo.
set -eu
cd "$(dirname "$0")"
mkdir -p dados resultados
pasta_binaria=$(mktemp -d)
trap 'rm -rf "$pasta_binaria"' EXIT

faltam_dados=0
for n in 1000 50000 100000 500000 1000000; do
    if [ ! -f "dados/dados_$n.txt" ]; then faltam_dados=1; fi
done
if [ "$faltam_dados" -eq 1 ]; then
    gcc -Wall -Wextra -std=c99 gerador.c -o "$pasta_binaria/gerador"
    "$pasta_binaria/gerador"
fi
gcc -Wall -Wextra -Werror -O2 -std=c99 benchmark_binaria_iterativa.c algoritmos.c -o "$pasta_binaria/benchmark"

cpu_binaria="sem afinidade fixa"
if command -v taskset >/dev/null 2>&1; then
    if afinidade_binaria=$(LC_ALL=C taskset -pc "$$" 2>/dev/null); then
        cpu_binaria=${afinidade_binaria##*: }
        cpu_binaria=${cpu_binaria%%,*}
        cpu_binaria=${cpu_binaria%%-*}
    fi
fi
if [ "$cpu_binaria" != "sem afinidade fixa" ] && taskset -c "$cpu_binaria" true 2>/dev/null; then
    echo "Medindo na CPU permitida $cpu_binaria."
    taskset -c "$cpu_binaria" "$pasta_binaria/benchmark"
else
    cpu_binaria="sem afinidade fixa"
    "$pasta_binaria/benchmark"
fi

CPU_BINARIA_MEDIDA="$cpu_binaria" python3 - <<'PY'
import hashlib, json, os, platform, subprocess
from datetime import datetime, timezone
from pathlib import Path
cpu = next((l.split(':', 1)[1].strip() for l in Path('/proc/cpuinfo').read_text().splitlines() if l.startswith('model name')), 'nao identificado')
fontes = ['algoritmos.c', 'algoritmos.h', 'benchmark_binaria_iterativa.c', 'gerador.c']
dados = {'registrado_em_utc': datetime.now(timezone.utc).isoformat(),
         'sistema': platform.platform(), 'cpu': cpu,
         'cpu_fixada': os.environ['CPU_BINARIA_MEDIDA'],
         'gcc': subprocess.check_output(['gcc', '--version'], text=True).splitlines()[0],
         'glibc': platform.libc_ver(), 'flags': '-Wall -Wextra -Werror -O2 -std=c99',
         'execucoes_por_tamanho': 100, 'relogio': 'CLOCK_MONOTONIC',
         'alvo': 'v[n-1] + 1', 'aquecimento_s': 0.3, 'chamadas_aquecimento': 3,
         'sha256_fontes': {f: hashlib.sha256(Path(f).read_bytes()).hexdigest() for f in fontes},
         'sha256_entradas': {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(Path('dados').glob('*.txt'))}}
Path('resultados/ambiente_binaria_iterativa.json').write_text(json.dumps(dados, ensure_ascii=False, indent=2) + '\n')
PY
echo "Concluido. Gere os graficos com: python3 grafico_binaria_iterativa.py"
