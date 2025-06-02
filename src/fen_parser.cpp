#include "fen_parser.h"
#include <sstream>
#include <iostream>

std::vector<std::vector<FENParser::Position>> FENParser::parseFEN(const std::string& fen) {
    std::vector<std::vector<Position>> board(8, std::vector<Position>(8));
    
    // Nur den ersten Teil der FEN-Notation verwenden (Brettzustand)
    std::string boardPart = fen.substr(0, fen.find(' '));
    
    int rank = 7; // Starte bei Rang 8 (Index 7)
    int file = 0; // Starte bei Datei a (Index 0)
    
    for (char c : boardPart) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (std::isdigit(c)) {
            // Leere Felder überspringen
            int emptySquares = c - '0';
            file += emptySquares;
        } else {
            // Figur setzen
            if (file < 8 && rank >= 0) {
                PieceType piece = getPieceFromChar(c);
                PieceColor color = getColorFromChar(c);
                board[file][rank] = Position(piece, color);
            }
            file++;
        }
    }
    
    return board;
}

std::string FENParser::positionToFEN(const std::vector<std::vector<Position>>& board) {
    std::string fen = "";
    
    for (int rank = 7; rank >= 0; rank--) {
        int emptyCount = 0;
        
        for (int file = 0; file < 8; file++) {
            const Position& pos = board[file][rank];
            
            if (pos.isEmpty) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                
                char pieceChar = ' ';
                switch (pos.piece) {
                    case PieceType::PAWN: pieceChar = 'p'; break;
                    case PieceType::ROOK: pieceChar = 'r'; break;
                    case PieceType::KNIGHT: pieceChar = 'n'; break;
                    case PieceType::BISHOP: pieceChar = 'b'; break;
                    case PieceType::QUEEN: pieceChar = 'q'; break;
                    case PieceType::KING: pieceChar = 'k'; break;
                    default: break;
                }
                
                if (pos.color == PieceColor::WHITE) {
                    pieceChar = std::toupper(pieceChar);
                }
                
                fen += pieceChar;
            }
        }
        
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        
        if (rank > 0) fen += '/';
    }
    
    return fen;
}

PieceColor FENParser::getColorFromChar(char c) {
    return std::isupper(c) ? PieceColor::WHITE : PieceColor::BLACK;
}

PieceType FENParser::getPieceFromChar(char c) {
    char lowerC = std::tolower(c);
    auto it = charToPieceType.find(lowerC);
    return (it != charToPieceType.end()) ? it->second : PieceType::NONE;
}