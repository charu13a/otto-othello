#include "board.hpp"

// Constructor
othelloBoard::othelloBoard() {
    this->positions.resize(64, 0);
}

// Display board: color is 1 for black, 2 for white
void othelloBoard::displayBoard(int color) {
    std::cout << "    A B C D E F G H" << std::endl;
    int row = 1;
    for (int i = 0; i < 64; i += 8) {
        // Plain green square
        std::cout << " " << row++ << " "
            << "\033[48;5;34m\033[38;5;232m \033[0m";

        for (int j = i; j < i+8; j++) {
            if (this->positions[j] == 1) {
                // Black disc
                std::cout << "\033[48;5;34m\033[38;5;232m\u2022 \033[0m";
            }
            else if (this->positions[j] == 2) {
                // White disc
                std::cout << "\033[48;5;34m\033[38;5;256m\u2022 \033[0m";
            }
            else if (this->moves.find(j) != this->moves.end() && color == 1) {
                // Black x
                std::cout << "\033[48;5;34m\033[38;5;232m\u2613 \033[0m";
            } 
            else if (this->moves.find(j) != this->moves.end() && color == 2) {
                // White x
                std::cout << "\033[48;5;34m\033[38;5;256m\u2613 \033[0m";
            }
            else {
                // Dot
                std::cout << "\033[48;5;34m\033[38;5;232m\u00B7 \033[0m";
            }
        }

        std::cout << std::endl;
    }
}

// Display legal moves for player
void othelloBoard::displayLegalMoves() {
    std::string colCoord = "ABCDEFGH";
    std::string rowCoord = "12345678";
    int colNum = 0;
    int rowNum = 0;
    int moveNum = 1;

    for (auto keyval : this->moves) {
        index2coord(keyval.first, colNum, rowNum);
        std::cout << "Legal move " << moveNum++ << ") "
            << colCoord[colNum] << rowCoord[colNum] << " ";

        std::list<int> discsFlipped = keyval.second;
        std::cout << "will flip discs: ";

        for (int disc : discsFlipped) {
            index2coord(disc, colNum, rowNum);
            std::cout << colCoord[colNum] << rowCoord[rowNum] << " " << std::endl;
        }

        std::cout << std::endl;
    }
}

// Finds all legal moves, returning a hash table with possible moves as keys,
// and a list of all pieces to be flipped as values.
void othelloBoard::findLegalMoves(int color) {
    for (int i = 0; i < 64; i++) {
        if (positions[i] == color) {
            // Check rows
            findLegalMoveInDirection(i, color, -1, moves);
            findLegalMoveInDirection(i, color, 1, moves);

            // Check columns
            findLegalMoveInDirection(i, color, -8, moves);
            findLegalMoveInDirection(i, color, 8, moves);

            // Check diagonals
            findLegalMoveInDirection(i, color, -9, moves);
            findLegalMoveInDirection(i, color, 9, moves);
            findLegalMoveInDirection(i, color, -7, moves);
            findLegalMoveInDirection(i, color, 7, moves);
        }
    }
}

// Helper function to find a legal move given a disc, its color and a direction.
// Stores legal move and flipped discs as a pair in the moves hash table.
void othelloBoard::findLegalMoveInDirection(int &disc, int &color, int direction,
        std::unordered_map<int, std::list<int>> &moves) {
    std::pair<int, std::list<int>> legalMove;
    std::list<int> flippedDiscs;
    int currentSquare = 0;
    int oppColor = (color == 1) ? 2 : 1;
    int row1 = 0, col1 = 0, row2 = 0, col2 = 0;

    for (int i = disc + direction; i < 64 || i > -1; i += direction) {
        // Guard against wrapping around the board
        index2coord(disc, col1, row1);
        index2coord(i, col2, row2);
        if (abs(col1 - col2) > 1 || abs(row1 - row2) > 1) {
            break;
        }

        // Keep moving in given direction, remembering any discs of the
        // opposite color. Break if we see any discs of our color.
        currentSquare = this->positions[i];
        if (currentSquare == color) {
            break;
        }
        else if (currentSquare == oppColor) {
            flippedDiscs.push_front(i);
            continue;
        }
        // If we see an empty square, it is a valid move: insert it into the
        // moves hash table. Second condition is to resolve edge case of
        // disc immediately adjacent to original disc.
        else if (currentSquare == 0 && this->positions[i - direction] == oppColor) {
            if (moves.find(i) != moves.end()) {
                moves.erase(i);
            }

            legalMove.first = i;
            legalMove.second = flippedDiscs;
            moves.insert(legalMove);
            break;
        }
    }
}

// Update positions after a move
void othelloBoard::updateBoard() {

}

// Helper function to convert board square index to coordinate strings
void othelloBoard::index2coord(int index, int &colNum, int &rowNum) {
    colNum = index % 8;
    rowNum = index / 8;
}
