#pragma once
#include "ChessBoard.h"

// 假设是我方回合，返回最佳行动
// 如果_maxScore不为nullptr，返回最佳行动的分数
Move getBestMove(const ChessBoard& board, int* _maxScore = nullptr, int searchDepth = 3);
// 得到对方最佳行动
// 如果_maxScore不为nullptr，返回对方最佳行动的分数（越大代表对方越有利）
Move getOpponentBestMove(const ChessBoard& board, int* _maxScore = nullptr, int searchDepth = 3);

// 当前行动方是否被将死
bool isCheckmate(const ChessBoard& board);
// getBestMove的多线程版本
Move getBestMoveMultiThread(const ChessBoard& board, int* _maxScore = nullptr, int searchDepth = 3);
// getOpponentBestMove的多线程版本
Move getOpponentBestMoveMultiThread(const ChessBoard& board, int* _maxScore = nullptr, int searchDepth = 3);
