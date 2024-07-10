package java_chess;

public class PieceMovement {

	public static String posibleK(int i) {

		int r = i / 8, c = i % 8;
		String list = "", oldPiece;

		// All the moves of knight x represent a rank and y represents a file
		int x[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
		int y[] = { 1, 2, 2, 1, -1, -2, -2, -1 };

		for (int j = 0; j < 8; j++) {
			r = i / 8;
			c = i % 8;
			r += x[j];
			c += y[j];

			if (r >= 0 && c >= 0 && r < 8 && c < 8) {// is move withing the board?
				oldPiece = Board.chessBoard[r][c];
				if (Board.chessBoard[r][c].equals(" ") || Character.isLowerCase(Board.chessBoard[r][c].charAt(0))) {
					Board.chessBoard[r][c] = "K";
					Board.chessBoard[i / 8][i % 8] = " ";

					if (kingSafe()) {
						list = list + (i / 8) + (i % 8) + r + c + oldPiece;
					}
					Board.chessBoard[r][c] = oldPiece;
					Board.chessBoard[i / 8][i % 8] = "K";
				}
			}
		}
		return list;
	}

	public static String posibleR(int i) {
		String list = "", oldPiece;
		int r = i / 8, c = i % 8;
		int temp = 1;
		for (int j = -1; j <= 1; j += 2) {
			// this while loop check posible moves in the rook's rank
			while (c + temp * j >= 0 && c + temp * j <= 7
					&& !Character.isUpperCase(Board.chessBoard[r][c + temp * j].charAt(0))) {
				oldPiece = Board.chessBoard[r][c + temp * j];
				Board.chessBoard[r][c] = " ";
				Board.chessBoard[r][c + temp * j] = "R";
				if (kingSafe()) {
					list = list + r + c + r + (c + temp * j) + oldPiece;
				}
				Board.chessBoard[r][c] = "R";
				Board.chessBoard[r][c + temp * j] = oldPiece;
				if (Character.isLowerCase(Board.chessBoard[r][c + temp * j].charAt(0)))
					break;
				temp++;
			}
			temp = 1;

			// this while loop check posible moves in the rook's rank
			while (r + temp * j >= 0 && r + temp * j <= 7
					&& !Character.isUpperCase(Board.chessBoard[r + temp * j][c].charAt(0))) {
				oldPiece = Board.chessBoard[r + temp * j][c];
				Board.chessBoard[r][c] = " ";
				Board.chessBoard[r + temp * j][c] = "R";
				if (kingSafe()) {
					list = list + r + c + (r + temp * j) + c + oldPiece;
				}
				Board.chessBoard[r][c] = "R";
				Board.chessBoard[r + temp * j][c] = oldPiece;
				if (Character.isLowerCase(Board.chessBoard[r + temp * j][c].charAt(0)))
					break;
				temp++;
			}
			temp = 1;
		}
		return list;
	}

	public static String posibleB(int i) {
		String list = "", oldPiece;
		int r = i / 8, c = i % 8;
		int temp = 1;
		for (int j = -1; j <= 1; j += 2) {
			for (int k = -1; k <= 1; k += 2) {
				while (r + temp * j >= 0 && r + temp * j <= 7 && c + temp * k >= 0 && c + temp * k <= 7
						&& !Character.isUpperCase(Board.chessBoard[r + temp * j][c + temp * k].charAt(0))) {
					oldPiece = Board.chessBoard[r + temp * j][c + temp * k];
					Board.chessBoard[r][c] = " ";
					Board.chessBoard[r + temp * j][c + temp * k] = "B";
					if (kingSafe()) {
						list = list + r + c + (r + temp * j) + (c + temp * k) + oldPiece;
					}
					Board.chessBoard[r][c] = "B";
					Board.chessBoard[r + temp * j][c + temp * k] = oldPiece;
					if (Character.isLowerCase(Board.chessBoard[r + temp * j][c + temp * k].charAt(0)))
						break;
					temp++;
				}
				temp = 1;
			}
		}
		return list;
	}

	public static String posibleQ(int i) {
		String list = "", oldPiece;
		int r = i / 8, c = i % 8;
		int temp = 1;
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				if (j != 0 || k != 0) {

					while (r + temp * j >= 0 && r + temp * j <= 7 && c + temp * k >= 0 && c + temp * k <= 7
							&& !Character.isUpperCase(Board.chessBoard[r + temp * j][c + temp * k].charAt(0))) {
						oldPiece = Board.chessBoard[r + temp * j][c + temp * k];
						Board.chessBoard[r][c] = " ";
						Board.chessBoard[r + temp * j][c + temp * k] = "Q";
						if (kingSafe())
							list = list + r + c + (r + temp * j) + (c + temp * k) + oldPiece;
						Board.chessBoard[r][c] = "Q";
						Board.chessBoard[r + temp * j][c + temp * k] = oldPiece;
						if (Character.isLowerCase(Board.chessBoard[r + temp * j][c + temp * k].charAt(0)))
							break;
						temp++;
					}
					temp = 1;
				}
			}
		}
		return list;
	}

	public static String posibleA(int i) {
		String list = "", oldPiece;
		int r = i / 8, c = i % 8;
		for (int j = 0; j < 9; j++) {
			if (j != 4) {
				int newRow = r - 1 + j / 3;
				int newCol = c - 1 + j % 3;
				if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
					if (Character.isLowerCase(Board.chessBoard[newRow][newCol].charAt(0))
							|| " ".equals(Board.chessBoard[newRow][newCol])) {
						oldPiece = Board.chessBoard[newRow][newCol];
						Board.chessBoard[r][c] = " ";
						Board.chessBoard[newRow][newCol] = "A";
						int kingTemp = Board.whiteKingPosition;
						Board.whiteKingPosition = i + (j / 3) * 8 + j % 3 - 9;
						if (kingSafe()) {
							list = list + r + c + newRow + newCol + oldPiece;
						}
						Board.chessBoard[r][c] = "A";
						Board.chessBoard[newRow][newCol] = oldPiece;
						Board.whiteKingPosition = kingTemp;
					}
				}
			}
		}
		// need to add casting later
		return list;
	}

	public static String posibleP(int i) {
		String list = "", oldPiece;
		int r = i / 8, c = i % 8;
		for (int j = -1; j <= 1; j += 2) {

			// move one up
			if (i >= 16 && Board.chessBoard[r - 1][c].equals(" ")) {
				oldPiece = Board.chessBoard[r - 1][c];
				Board.chessBoard[r][c] = " ";
				Board.chessBoard[r - 1][c] = "P";
				if (kingSafe())
					list = list + r + c + (r - 1) + c + oldPiece;
				Board.chessBoard[r][c] = "P";
				Board.chessBoard[r - 1][c] = oldPiece;
			}

			// move two up
			if (i >= 48 && Board.chessBoard[r - 1][c].equals(" ") && Board.chessBoard[r - 2][c].equals(" ")) {
				oldPiece = Board.chessBoard[r - 2][c];
				Board.chessBoard[r][c] = " ";
				Board.chessBoard[r - 2][c] = "P";
				if (kingSafe())
					list = list + r + c + (r - 2) + c + oldPiece;
				Board.chessBoard[r][c] = "P";
				Board.chessBoard[r - 2][c] = oldPiece;
			}

			// capture
			if (c + j >= 0 && c + j <= 7 && Character.isLowerCase(Board.chessBoard[r - 1][c + j].charAt(0))
					&& i >= 16) {
				oldPiece = Board.chessBoard[r - 1][c + j];
				Board.chessBoard[r][c] = " ";
				Board.chessBoard[r - 1][c + j] = "P";
				if (kingSafe())
					list = list + r + c + (r - 1) + (c + j) + oldPiece;
				Board.chessBoard[r][c] = "P";
				Board.chessBoard[r - 1][c + j] = oldPiece;
			}

			// promotion && capture
			if (c + j >= 0 && c + j <= 7 && Character.isLowerCase(Board.chessBoard[r - 1][c + j].charAt(0)) && i < 16) {
				String[] temp = { "Q", "R", "B", "K" };
				for (int k = 0; k < 4; k++) {
					oldPiece = Board.chessBoard[r - 1][c + j];
					Board.chessBoard[r][c] = " ";
					Board.chessBoard[r - 1][c + j] = temp[k];
					if (kingSafe())
						// column1,column2,captured-piece,new-piece,P
						list = list + c + (c + j) + oldPiece + temp[k] + "P";
					Board.chessBoard[r][c] = "P";
					Board.chessBoard[r - 1][c + j] = oldPiece;
				}
			}
		}

		// promotion && no capture
		if (Board.chessBoard[r - 1][c].equals(" ") && i < 16) {
			String[] temp = { "Q", "R", "B", "K" };
			for (int k = 0; k < 4; k++) {
				oldPiece = Board.chessBoard[r - 1][c];
				Board.chessBoard[r][c] = " ";
				Board.chessBoard[r - 1][c] = temp[k];
				if (kingSafe())
					// column1,column2,captured-piece,new-piece,P
					list = list + c + c + oldPiece + temp[k] + "P";
				Board.chessBoard[r][c] = "P";
				Board.chessBoard[r - 1][c] = oldPiece;
			}
		}
		return list;

		// to do => en passant
	}

	public static boolean kingSafe() {
		int r = Board.whiteKingPosition / 8, c = Board.whiteKingPosition % 8;
		int temp = 1;

		// check diagonal threats from bishop/queen
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				while (r + temp * i <= 7 && r + temp * i >= 0 && c + temp * j <= 7 && c + temp * j >= 0
						&& Board.chessBoard[r + temp * i][c + temp * j].equals(" "))
					temp++;
				if ((r + temp * i <= 7 && r + temp * i >= 0 && c + temp * j <= 7 && c + temp * j >= 0)
						&& (Board.chessBoard[r + temp * i][c + temp * j].equals("b")
								|| Board.chessBoard[r + temp * i][c + temp * j].equals("q")))
					return false;
				temp = 1;
			}
		}

		// check streight threats from rook/queen
		for (int i = -1; i <= 1; i += 2) {
			// check threats in a rank
			while (c + temp * i >= 0 && c + temp * i <= 7 && Board.chessBoard[r][c + temp * i].equals(" "))
				temp++;
			if ((c + temp * i >= 0 && c + temp * i <= 7)
					&& (Board.chessBoard[r][c + temp * i].equals("r") || Board.chessBoard[r][c + temp * i].equals("q")))
				return false;
			temp = 1;

			// check threats in a file
			while (r + temp * i >= 0 && r + temp * i <= 7 && Board.chessBoard[r + temp * i][c].equals(" "))
				temp++;
			if ((r + temp * i >= 0 && r + temp * i <= 7)
					&& (Board.chessBoard[r + temp * i][c].equals("r") || Board.chessBoard[r + temp * i][c].equals("q")))
				return false;
			temp = 1;
		}

		// check threats from a knight
		// All the moves of knight. x represent a rank and y represents a file
		int x[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
		int y[] = { 1, 2, 2, 1, -1, -2, -2, -1 };

		for (int j = 0; j < 8; j++) {

			while (r + x[j] >= 0 && r + x[j] <= 7 && c + y[j] <= 7 && c + y[j] >= 0) {

				if (Board.chessBoard[r + x[j]][c + y[j]].equals("k"))
					return false;
				break;
			}
		}

		// check threats from pawn
		if (Board.whiteKingPosition >= 16) {
			/*
			 * here we are checking if white king position is atleast 16 becuase if our
			 * white king is in the 6th or 7th rank or position <= 16 then it is always safe
			 * from enemy pawns since pawns can only move forward and all the pawns are on
			 * the 6th rank at the starting of game
			 */

			if (c > 0 && Board.chessBoard[r - 1][c - 1].equals("p"))
				return false;

			if (c < 7 && Board.chessBoard[r - 1][c + 1].equals("p"))
				return false;
		}

		// king
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (i != 0 || j != 0) {
					if (r + i >= 0 && r + i <= 7 && c + j >= 0 && c + j <= 7
							&& Board.chessBoard[r + i][c + j].equals("a"))
						return false;
				}
			}
		}
		return true;
	}
}