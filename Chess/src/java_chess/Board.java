package java_chess;

public class Board {
	
	static String chessBoard[][] = { 
			{ "r", "k", "b", "q", "a", "b", "k", "r" },
			{ "p", "p", "p", "p", "p", "p", "p", "p" },
			{ " ", " ", " ", " ", " ", " ", " ", " " },
			{ " ", " ", " ", " ", " ", " ", " ", " " }, 
			{ " ", " ", " ", " ", " ", " ", " ", " " },
			{ " ", " ", " ", " ", " ", " ", " ", " " },
			{ "P", "P", "P", "P", "P", "P", "P", "P" },
			{ "R", "K", "B", "Q", "A", "B", "K", "R" } };
	
	static int whiteKingPosition, blackKingPosition;


	public static void flipBoard() {
		String temp;
		for (int i = 0; i < 32; i++) {
			int r = i / 8, c = i % 8;
			if (Character.isUpperCase(chessBoard[r][c].charAt(0))) {
				temp = chessBoard[r][c].toLowerCase();
			} else {
				temp = chessBoard[r][c].toUpperCase();
			}
			if (Character.isUpperCase(chessBoard[7 - r][7 - c].charAt(0))) {
				chessBoard[r][c] = chessBoard[7 - r][7 - c].toLowerCase();
			} else {
				chessBoard[r][c] = chessBoard[7 - r][7 - c].toUpperCase();
			}
			chessBoard[7 - r][7 - c] = temp;
		}
		int kingTemp = whiteKingPosition;
		whiteKingPosition = 63 - blackKingPosition;
		blackKingPosition = 63 - kingTemp;
	}

	public static void makeMove(String move) {
		if (move.charAt(4) != 'P') {
			chessBoard[Character.getNumericValue(move.charAt(2))][Character
					.getNumericValue(move.charAt(3))] = chessBoard[Character.getNumericValue(move.charAt(0))][Character
							.getNumericValue(move.charAt(1))];
			chessBoard[Character.getNumericValue(move.charAt(0))][Character.getNumericValue(move.charAt(1))] = " ";
			if ("A".equals(
					chessBoard[Character.getNumericValue(move.charAt(2))][Character.getNumericValue(move.charAt(3))])) {
				whiteKingPosition = 8 * Character.getNumericValue(move.charAt(2))
						+ Character.getNumericValue(move.charAt(3));
			}
		} else {
			// if pawn promotion
			chessBoard[1][Character.getNumericValue(move.charAt(0))] = " ";
			chessBoard[0][Character.getNumericValue(move.charAt(1))] = String.valueOf(move.charAt(3));
		}
	}

	public static void undoMove(String move) {
		if (move.charAt(4) != 'P') {
			chessBoard[Character.getNumericValue(move.charAt(0))][Character
					.getNumericValue(move.charAt(1))] = chessBoard[Character.getNumericValue(move.charAt(2))][Character
							.getNumericValue(move.charAt(3))];
			chessBoard[Character.getNumericValue(move.charAt(2))][Character.getNumericValue(move.charAt(3))] = String
					.valueOf(move.charAt(4));
			if ("A".equals(
					chessBoard[Character.getNumericValue(move.charAt(0))][Character.getNumericValue(move.charAt(1))])) {
				whiteKingPosition = 8 * Character.getNumericValue(move.charAt(0))
						+ Character.getNumericValue(move.charAt(1));
			}
		} else {
			// if pawn promotion
			chessBoard[1][Character.getNumericValue(move.charAt(0))] = "P";
			chessBoard[0][Character.getNumericValue(move.charAt(1))] = String.valueOf(move.charAt(2));
		}
	}


}
