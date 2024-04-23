#pragma once
#include <vector>
#include "ChessPiece.h"


// 棋力评估
int ChessPiecePositionEvaluation(ChessPieceType pieceType, int x, int y);

// 行棋可能性评估
int ChessPieceMoveEvaluation(const std::vector<ChessPieceType>& chessPiecesCanBeCaptured);

// 棋子价值评估
int ChessPieceValueEvaluation(ChessPieceType pieceType);
