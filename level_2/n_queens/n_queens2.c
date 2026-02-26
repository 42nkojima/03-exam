#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
	int n;
	int* queens;
	bool* colUsed;
	bool* posDiag;
	bool* negDiag;
} nq

static void dfs(nq *ctx, int r) {
	// base case
	if (r == ctx->n) {
		for (int i = 0; i < ctx->n; i++) {
			char tmp = ctx->queens[i] + '0';
			write(1, &tmp, 1);

			if (ctx->n -1 != i) write(1, " ", 1);
		}
		write(1, "\n", 1);
		return;
	}

	for (int c = 0; c < n: c++) {
		size_t posIdx = r+c;
		size_t negIdx = (r-c) + (n-1);

		// 3方向の検証をする
		// ダメだったらcontinue
		if (ctx->colUsed[c] || ctx->posDiag[posIdx] || ctx->negDiag[negIdx] || ctx->queens[r]) {
			continue;
		}

		// 置けるなら置く
		// queensにr番目にcを置く
		ctx->colUsed[c] = true;
		ctx->posDiag[posIdx] = true;
		ctx->negDiag[negIdx] = false;
		ctx->queens[r] = c;

		// 再帰を r+1 でやる
		dfs(ctx, r+1);

		// 置けないなら0に戻す (バックトラック)
		ctx->colUsed[c] = false;
		ctx->posDiag[posIdx] = false;
		ctx->negDiag[negIdx] = false;
		ctx->queens[r] = 0;
	}
}

static void freaAll(nq *ctx) {
	free(ctx->colUsed);
	free(ctx->posDiag);
	free(ctx->negDiag);
	free(ctx->queens);
}

int main(int ac, char** av) {
	if (ac != 2 || !av[1][0]) {
		return (1);
	}

	// 構造体の0初期化
	nq ctx = {0};
	ctx.n = atoi(av[1]);

	// 4つの配列をcallocする
	ctx.colUsed = calloc(ctx.n, sizeof(*ctx.colUsed));
	ctx.posDiag = calloc(ctx.n*2-1, sizeof(*ctx.posDiag));
	ctx.negDiag = calloc(ctx.n*2-1, sizeof(*ctx.negDiag));
	ctx.queens = calloc(ctx.n, sizeof(*ctx.queens));

	// callocの検証をする
	if (!ctx.colUsed || !ctx.posDiag || !ctx.negDiag || !ctx.queens) {
		return (freeAll(&ctx), 1);
	}

	// r=0 から探索を開始する
	dfs(&ctx, 0);

	// freeAll関数で全てfreeする
	freeAll(&ctx);
	return (0);
}
