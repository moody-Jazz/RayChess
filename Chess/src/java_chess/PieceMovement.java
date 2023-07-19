package java_chess;

public class PieceMovement {

	public static String posibleK(int i) {

		int r = i / 8, c = i % 8;
		String list = "", oldPiece;

		// All the moves of knight x represent a rank and y represents a file and
		int x[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
		int y[] = { 1, 2, 2, 1, -1, -2, -2, -1 };

		for (int j = 0; j < 8; j++) {
			r = i / 8;
			c = i % 8;
			r += x[j];
			c += y[j];

			if (r >= 0 && c >= 0 && r < 8 && c < 8) {// is move withing the board? if yes then [r][c] represent a
														// potentially legal move
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
				try {
					if (Character.isLowerCase(Board.chessBoard[r - 1 + j / 3][c - 1 + j % 3].charAt(0))
							|| " ".equals(Board.chessBoard[r - 1 + j / 3][c - 1 + j % 3])) {
						oldPiece = Board.chessBoard[r - 1 + j / 3][c - 1 + j % 3];
						Board.chessBoard[r][c] = " ";
						Board.chessBoard[r - 1 + j / 3][c - 1 + j % 3] = "A";
						int kingTemp = Board.whiteKingPosition;
						Board.whiteKingPosition = i + (j / 3) * 8 + j % 3 - 9;
						if (kingSafe()) {
							list = list + r + c + (r - 1 + j / 3) + (c - 1 + j % 3) + oldPiece;
						}
						Board.chessBoard[r][c] = "A";
						Board.chessBoard[r - 1 + j / 3][c - 1 + j % 3] = oldPiece;
						Board.whiteKingPosition = kingTemp;
					}
				} catch (Exception e) {
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
			try {// capture
				if (Character.isLowerCase(Board.chessBoard[r - 1][c + j].charAt(0)) && i >= 16) {
					oldPiece = Board.chessBoard[r - 1][c + j];
					Board.chessBoard[r][c] = " ";
					Board.chessBoard[r - 1][c + j] = "P";
					if (kingSafe()) {
						list = list + r + c + (r - 1) + (c + j) + oldPiece;
					}
					Board.chessBoard[r][c] = "P";
					Board.chessBoard[r - 1][c + j] = oldPiece;
				}
			} catch (Exception e) {
			}
			try {// promotion && capture
				if (Character.isLowerCase(Board.chessBoard[r - 1][c + j].charAt(0)) && i < 16) {
					String[] temp = { "Q", "R", "B", "K" };
					for (int k = 0; k < 4; k++) {
						oldPiece = Board.chessBoard[r - 1][c + j];
						Board.chessBoard[r][c] = " ";
						Board.chessBoard[r - 1][c + j] = temp[k];
						if (kingSafe()) {
							// column1,column2,captured-piece,new-piece,P
							list = list + c + (c + j) + oldPiece + temp[k] + "P";
						}
						Board.chessBoard[r][c] = "P";
						Board.chessBoard[r - 1][c + j] = oldPiece;
					}
				}
			} catch (Exception e) {
			}
		}
		try {// move one up
			if (" ".equals(Board.chessBoard[r - 1][c]) && i >= 16) {
				oldPiece = Board.chessBoard[r - 1][c];
				Board.chessBoard[r][c] = " ";
				Board.chessBoard[r - 1][c] = "P";
				if (kingSafe()) {
					list = list + r + c + (r - 1) + c + oldPiece;
				}
				Board.chessBoard[r][c] = "P";
				Board.chessBoard[r - 1][c] = oldPiece;
			}
		} catch (Exception e) {
		}
		try {// promotion && no capture
			if (" ".equals(Board.chessBoard[r - 1][c]) && i < 16) {
				String[] temp = { "Q", "R", "B", "K" };
				for (int k = 0; k < 4; k++) {
					oldPiece = Board.chessBoard[r - 1][c];
					Board.chessBoard[r][c] = " ";
					Board.chessBoard[r - 1][c] = temp[k];
					if (kingSafe()) {
						// column1,column2,captured-piece,new-piece,P
						list = list + c + c + oldPiece + temp[k] + "P";
					}
					Board.chessBoard[r][c] = "P";
					Board.chessBoard[r - 1][c] = oldPiece;
				}
			}
		} catch (Exception e) {
		}
		try {// move two up
			if (" ".equals(Board.chessBoard[r - 1][c]) && " ".equals(Board.chessBoard[r - 2][c]) && i >= 48) {
				oldPiece = Board.chessBoard[r - 2][c];
				Board.chessBoard[r][c] = " ";
				Board.chessBoard[r - 2][c] = "P";
				if (kingSafe()) {
					list = list + r + c + (r - 2) + c + oldPiece;
				}
				Board.chessBoard[r][c] = "P";
				Board.chessBoard[r - 2][c] = oldPiece;
			}
		} catch (Exception e) {
		}
		return list;
	}

	public static boolean kingSafe() {
		// bishop/queen
		int temp = 1;
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				try {
					while (" "
							.equals(Board.chessBoard[Board.whiteKingPosition / 8 + temp * i][Board.whiteKingPosition % 8
									+ temp * j])) {
						temp++;
					}
					if ("b".equals(Board.chessBoard[Board.whiteKingPosition / 8 + temp * i][Board.whiteKingPosition % 8
							+ temp * j])
							|| "q".equals(
									Board.chessBoard[Board.whiteKingPosition / 8 + temp * i][Board.whiteKingPosition % 8
											+ temp * j])) {
						return false;
					}
				} catch (Exception e) {
				}
				temp = 1;
			}
		}
		// rook/queen
		for (int i = -1; i <= 1; i += 2) {
			try {
				while (" ".equals(
						Board.chessBoard[Board.whiteKingPosition / 8][Board.whiteKingPosition % 8 + temp * i])) {
					temp++;
				}
				if ("r".equals(Board.chessBoard[Board.whiteKingPosition / 8][Board.whiteKingPosition % 8 + temp * i])
						|| "q".equals(Board.chessBoard[Board.whiteKingPosition / 8][Board.whiteKingPosition % 8
								+ temp * i])) {
					return false;
				}
			} catch (Exception e) {
			}
			temp = 1;
			try {
				while (" ".equals(
						Board.chessBoard[Board.whiteKingPosition / 8 + temp * i][Board.whiteKingPosition % 8])) {
					temp++;
				}
				if ("r".equals(Board.chessBoard[Board.whiteKingPosition / 8 + temp * i][Board.whiteKingPosition % 8])
						|| "q".equals(Board.chessBoard[Board.whiteKingPosition / 8 + temp * i][Board.whiteKingPosition
								% 8])) {
					return false;
				}
			} catch (Exception e) {
			}
			temp = 1;
		}
		// knight
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				try {
					if ("k".equals(
							Board.chessBoard[Board.whiteKingPosition / 8 + i][Board.whiteKingPosition % 8 + j * 2])) {
						return false;
					}
				} catch (Exception e) {
				}
				try {
					if ("k".equals(
							Board.chessBoard[Board.whiteKingPosition / 8 + i * 2][Board.whiteKingPosition % 8 + j])) {
						return false;
					}
				} catch (Exception e) {
				}
			}
		}
		// pawn
		if (Board.whiteKingPosition >= 16) {
			try {
				if ("p".equals(Board.chessBoard[Board.whiteKingPosition / 8 - 1][Board.whiteKingPosition % 8 - 1])) {
					return false;
				}
			} catch (Exception e) {
			}
			try {
				if ("p".equals(Board.chessBoard[Board.whiteKingPosition / 8 - 1][Board.whiteKingPosition % 8 + 1])) {
					return false;
				}
			} catch (Exception e) {
			}
			// king
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i != 0 || j != 0) {
						try {
							if ("a".equals(Board.chessBoard[Board.whiteKingPosition / 8 + i][Board.whiteKingPosition % 8
									+ j])) {
								return false;
							}
						} catch (Exception e) {
						}
					}
				}
			}
		}
		return true;
	}
}