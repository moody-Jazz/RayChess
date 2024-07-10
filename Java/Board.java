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

	static int whiteKingPosition = 60 , blackKingPosition = 4;

	public static void flipBoard() {
		String temp;
		for (int i = 0; i < 32; i++) {
			int r = i / 8, c = i % 8;
			if (Character.isUpperCase(chessBoard[r][c].charAt(0)))
				temp = chessBoard[r][c].toLowerCase();
			else
				temp = chessBoard[r][c].toUpperCase();

			if (Character.isUpperCase(chessBoard[7 - r][7 - c].charAt(0)))
				chessBoard[r][c] = chessBoard[7 - r][7 - c].toLowerCase();
			else
				chessBoard[r][c] = chessBoard[7 - r][7 - c].toUpperCase();

			chessBoard[7 - r][7 - c] = temp;
		}
		int kingTemp = whiteKingPosition;
		whiteKingPosition = 63 - blackKingPosition;
		blackKingPosition = 63 - kingTemp;
	}

	public static void makeMove(String move) {
		// if move is not pawn promotion format of move string =
		// initalx_initialy_finalx_final y_pieceOnFinalxy
		if (move.charAt(4) != 'P') {
			int initialRank = Character.getNumericValue(move.charAt(0));
			int initialFile = Character.getNumericValue(move.charAt(1));
			int finalRank = Character.getNumericValue(move.charAt(2));
			int finalFile = Character.getNumericValue(move.charAt(3));

			chessBoard[finalRank][finalFile] = chessBoard[initialRank][initialFile];
			chessBoard[initialRank][initialFile] = " ";

			if (chessBoard[finalRank][finalFile].equals("A"))
				// if the moved piece was king also update its position in whiteKingPosition
				whiteKingPosition = 8 * finalRank + finalFile;
		} else {
			// if pawn promotion format of move string =
			// initialY_finaly_capturedPiece_promotedPiece_P
			chessBoard[1][Character.getNumericValue(move.charAt(0))] = " ";// inital square before pawn movement = " "
			// square on which pawn moved+promoted = _promotedPiece
			chessBoard[0][Character.getNumericValue(move.charAt(1))] = String.valueOf(move.charAt(3));
		}
	}

	public static void undoMove(String move) {

		// if move is not pawn promotion format of move string =
		// initalx_initialy_finalx_final y_pieceOnFinalxy
		if (move.charAt(4) != 'P') {
			int initialRank = Character.getNumericValue(move.charAt(0));
			int initialFile = Character.getNumericValue(move.charAt(1));
			int finalRank = Character.getNumericValue(move.charAt(2));
			int finalFile = Character.getNumericValue(move.charAt(3));
			String capturedPiece = String.valueOf(move.charAt(4));

			chessBoard[initialRank][initialFile] = chessBoard[finalRank][finalFile];
			chessBoard[finalRank][finalFile] = capturedPiece;

			if (chessBoard[initialRank][initialFile].equals("A"))
				whiteKingPosition = 8 * initialRank + initialFile;

		} else {
			// if pawn promotion
			chessBoard[1][Character.getNumericValue(move.charAt(0))] = "P";
			chessBoard[0][Character.getNumericValue(move.charAt(1))] = String.valueOf(move.charAt(2));
		}
	}

}
