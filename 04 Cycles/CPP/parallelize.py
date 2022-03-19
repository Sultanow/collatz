import argparse, sys, subprocess, threading, multiprocessing.pool, json

def Run(nargs):
    q_begin, q_end, x0_begin, x0_end, n = [nargs[k] for k in 'q_begin q_end x0_begin x0_end n'.split()]
    p = subprocess.run(['a.exe', f'--q-begin={q_begin}', f'--q-end={q_end}', f'--x0-begin={x0_begin}', f'--x0-end={x0_end}', f'--n={n}'], check = True, capture_output = True)
    with open(f'cycles.{str(q_begin).zfill(6)}_{str(q_end).zfill(6)}.{str(x0_begin).zfill(6)}_{str(x0_end).zfill(6)}.{str(n).zfill(6)}', 'wb') as f:
        f.write(p.stdout)

parser = argparse.ArgumentParser()
parser.add_argument('--q-begin', type = int, help = 'Q search range begin')
parser.add_argument('--q-end', type = int, help = 'Q search range end')
parser.add_argument('--x0-begin', type = int, help = 'X0 search range begin')
parser.add_argument('--x0-end', type = int, help = 'X0 search range end')
parser.add_argument('--n', type = int, help = 'N')
args = parser.parse_args()

with multiprocessing.pool.ThreadPool(multiprocessing.cpu_count()) as pool:
    block_q = (args.q_end - args.q_begin + 3) // 4
    block_x0 = (args.x0_end - args.x0_begin + 3) // 4
    pool.map(Run,
        [dict(q_begin = i, q_end = min(i + block_q, args.q_end), x0_begin = j, x0_end = min(j + block_x0, args.x0_begin), n = args.n)
            for i in range(args.q_begin, args.q_end, block_q) for j in range(0, args.x0_begin, block_x0)] +
        [dict(q_begin = i, q_end = min(i + block_q, args.q_begin), x0_begin = j, x0_end = min(j + block_x0, args.x0_end), n = args.n)
            for i in range(0, args.q_begin, block_q) for j in range(args.x0_begin, args.x0_end, block_x0)] +
        [dict(q_begin = i, q_end = min(i + block_q, args.q_end), x0_begin = j, x0_end = min(j + block_x0, args.x0_end), n = args.n)
            for i in range(args.q_begin, args.q_end, block_q) for j in range(args.x0_begin, args.x0_end, block_x0)],
    )