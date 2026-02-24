#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * 1行ずつクイーンを置く (各行に置ける数は必ず1つ)
 *
 * 置けるかどうかの判定は3種類でする
 * - colUsed[c] 列cがすでに使われているかどうか
 * - negDiagUsed[idx] 斜め(\) が既に使われてるか
 * - posDiagUsed[idx] 斜め(/) が既に使われてるか
 *
 * 対角線の添字
 * - negDiagは、同じ数字になる時はr-c つまり n=4のとき -3~3
 *   0からにしたいからずらして、0~2n-2
 * - posDiagは、同様に r+cでよくて、負の数はないからそのまま使う 0~2n-2
 */

typedef struct {
	int n; 			   // 盤面のサイズ
	bool* colUsed;	   // 列cの使用状況
	bool* negDiagUsed; // (\)対角線の使用状況 (r-c) + (n-1)
	bool* posDiagUsed; // (/)対角線の使用状況 (r+c)
	int* queenPos;	   // 各行に置いたクイーンの列
} NQueenCtx;

static void dfs(NQueenCtx *ctx, int r) {
	if (r == ctx->n) {
		for (int i = 0; i < ctx->n; i++) {
			char tmp = ctx->queenPos[i] + '0';
			write(1, &tmp, 1);
			if (i != ctx->n - 1) {
				write(1, " ", 1);
			}
		}
		write(1, "\n", 1);
		return;
	}

	for (int c = 0; c < ctx->n; c++) {
		size_t negIdx = (r-c) + (ctx->n-1);
		size_t posIdx = r+c;

		if (ctx->colUsed[c] || ctx->negDiagUsed[negIdx] || ctx->posDiagUsed[posIdx]) {
			continue;
		}

		ctx->colUsed[c] = true;
		ctx->negDiagUsed[negIdx] = true;
		ctx->posDiagUsed[posIdx] = true;
		ctx->queenPos[r] = c;

		dfs(ctx, r + 1);

		ctx->colUsed[c] = false;
		ctx->negDiagUsed[negIdx] = false;
		ctx->posDiagUsed[posIdx] = false;
		ctx->queenPos[r] = 0;
	}
}

static void freeAll(NQueenCtx *ctx) {
	free(ctx->colUsed);
	free(ctx->negDiagUsed);
	free(ctx->posDiagUsed);
	free(ctx->queenPos);
}

int main(int ac, char** av) {
	if (ac != 2 || !av[1][0])
		return (1);

	NQueenCtx ctx = {0};
	ctx.n = atoi(av[1]);

	ctx.colUsed = calloc(ctx.n, sizeof(*ctx.colUsed));
	ctx.negDiagUsed = calloc(2*ctx.n - 1, sizeof(*ctx.negDiagUsed));
	ctx.posDiagUsed = calloc(2*ctx.n - 1, sizeof(*ctx.posDiagUsed));
	ctx.queenPos = calloc(ctx.n, sizeof(*ctx.queenPos));

	if (!ctx.colUsed || !ctx.negDiagUsed || !ctx.posDiagUsed || !ctx.queenPos) {
		return (freeAll(&ctx), 0);
	}

	dfs(&ctx, 0);

	freeAll(&ctx);
	return (0);
}
