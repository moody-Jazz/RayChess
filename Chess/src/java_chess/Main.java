package java_chess;

import java.util.Arrays;
import javax.swing.JFrame;

public class Main {

	/**
	 * In the 2d array of string chessBoard small letters represents black pieces
	 * and capital letters represents white pieces " " represents empty tiles which
	 * currently doesn't have any piece example p = black pawn, k = black knight
	 * similarly P = white pawn and R = white rook king and knight have same initial
	 * letter thats why kings are represented by A and a
	 * 
	 */
	static String chessBoard[][] = { { "r", "k", "b", "q", "a", "b", "k", "r" },
			{ "p", "p", "p", "p", "p", "p", "p", "p" }, { " ", " ", " ", " ", " ", " ", " ", " " },
			{ " ", " ", " ", " ", " ", " ", " ", " " }, { " ", " ", " ", " ", " ", " ", " ", " " },
			{ " ", " ", " ", " ", " ", " ", " ", " " }, { "P", "P", "P", "P", "P", "P", "P", "P" },
			{ "R", "K", "B", "Q", "A", "B", "K", "R" } };

	static int whiteKingPosition;
	static int blackKingPosition;
	static int globalDepth = 4;

	public static void main(String[] args) {

		// the loops below find the initail position of the black and white kings
		while (!"A".equals(chessBoard[whiteKingPosition / 8][whiteKingPosition % 8]))
			whiteKingPosition++;

		while (!"a".equals(chessBoard[blackKingPosition / 8][blackKingPosition % 8]))
			blackKingPosition++;

		
		 JFrame mainFrame = new JFrame("Chess"); 
		 Board board = new Board();
		 mainFrame.add(board); 
		 mainFrame.setSize(800,800);
		 mainFrame.setVisible(true);
		 mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		 
		 
		System.out.println(possibleMoves());
		makeMove(alphaBeta(globalDepth, 1000000, -1000000, "", 0));
		for (int i = 0; i < 8; i++)
			System.out.println(Arrays.toString(chessBoard[i]));
	}

	public static String alphaBeta(int depth, int beta, int alpha, String move, int player) {

		String list = possibleMoves();

		if (depth == 0 || list.length() == 0)
			return move + (rating() * player * 2 - 1);

		player = 1 - player;// flips player to 1 and 0 then player*2-1 is either 1 or -1

		for (int i = 0; i < list.length(); i += 5) {
			makeMove(list.substring(i, i + 5));
			flipBoard();
			String returnString = alphaBeta(depth - 1, beta, alpha, list.substring(i, i + 5), player);
			int value = Integer.valueOf(returnString.substring(5));
			flipBoard();
			undoMove(list.substring(i, i + 5));
			if (player == 0) {
				if (value <= beta) {
					beta = value;
					if (depth == globalDepth) {
						move = returnString.substring(0, 5);
					}
				}

			} else {
				if (value > alpha) {
					alpha = value;
					if (depth == globalDepth) {
						move = returnString.substring(0, 5);
					}
				}
			}
			if (alpha >= beta) {
				if (player == 0)
					return move + beta;
				else
					return move + alpha;
			}
		}
		if (player == 0)
			return move + beta;
		else
			return move + alpha;
	}

	public static void flipBoard() {
		String temp;
		for (int i = 0; i < 32; i++) {
			int row = i / 8;
			int col = i % 8;
			if (Character.isUpperCase(chessBoard[row][col].charAt(0)))
				temp = chessBoard[row][col].toLowerCase();
			else
				temp = chessBoard[row][col].toUpperCase();

			if (Character.isUpperCase(chessBoard[7 - row][7 - col].charAt(0)))
				chessBoard[row][col] = chessBoard[7 - row][7 - col].toLowerCase();
			else
				chessBoard[row][col] = chessBoard[7 - row][7 - col].toUpperCase();
			chessBoard[7 - row][7 - col] = temp;

		}
		int kingTemp = whiteKingPosition;
		whiteKingPosition = 63 - blackKingPosition;
		blackKingPosition = 63 - kingTemp;
	}

	public static int rating() {

		return 0;
	}

	public static void makeMove(String move) {

//		making a regular moves
		if (move.charAt(4) != 'P') {
			chessBoard[Character.getNumericValue(move.charAt(2))][Character
					.getNumericValue(move.charAt(3))] = chessBoard[Character.getNumericValue(move.charAt(0))][Character
							.getNumericValue(move.charAt(1))];
			chessBoard[Character.getNumericValue(move.charAt(0))][Character.getNumericValue(move.charAt(1))] = " ";

			if ("A".equals(
					chessBoard[Character.getNumericValue(move.charAt(2))][Character.getNumericValue(move.charAt(3))]))
				whiteKingPosition = 8 * Character.getNumericValue(move.charAt(2))
						+ Character.getNumericValue(move.charAt(3));

		} else {// making a pawn promotion move
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
					chessBoard[Character.getNumericValue(move.charAt(0))][Character.getNumericValue(move.charAt(0))]))
				whiteKingPosition = 8 * Character.getNumericValue(move.charAt(0))
						+ Character.getNumericValue(move.charAt(1));

		} else {// making a pawn promotion move
			chessBoard[1][Character.getNumericValue(move.charAt(0))] = "P";
			chessBoard[0][Character.getNumericValue(move.charAt(1))] = String.valueOf(move.charAt(2));
		}
	}

	public static String possibleMoves() {
		String list = "";
		for (int i = 0; i < 64; i++) {
			switch (chessBoard[i / 8][i % 8]) {
			case "P":
				list += possibleP(i);
				break;
			case "R":
				list += possibleR(i);
				break;
			case "K":
				list += possibleK(i);
				break;
			case "B":
				list += possibleB(i);
				break;
			case "Q":
				list += possibleQ(i);
				break;
			case "A":
				list += possibleA(i);
				break;
			}
		}
		return list;// x1,y1,x2,y2,captured piece
	}

	// This method Finds all the possible moves of pawns
	public static String possibleP(int i) {
		String list = "";
		String oldPiece;
		int row = i / 8;
		int col = i % 8;
		for (int j = -1; j <= 1; j += 2) {

			// possible diagonal captures
			try {
				if (Character.isLowerCase(chessBoard[row - 1][col + j].charAt(0)) && i >= 16) {
					oldPiece = chessBoard[row - 1][col + j];
					chessBoard[row][col] = " ";
					chessBoard[row - 1][col + j] = "P";
					if (isKingSafe())
						list = list + row + col + (row - 1) + (col + j) + oldPiece;
					chessBoard[row][col] = "P";
					chessBoard[row - 1][col + j] = oldPiece;
				}

			} catch (Exception e) {
			}

			// capture + promotion for pawn
			try {
				if (Character.isLowerCase(chessBoard[row - 1][col + j].charAt(0)) && i < 16) {
					String[] promotionOptions = { "Q", "R", "B", "K" };
					for (int k = 0; k < promotionOptions.length; k++) {
						oldPiece = chessBoard[row - 1][col + j];
						chessBoard[row][col] = " ";
						chessBoard[row - 1][col + j] = promotionOptions[k];

						if (isKingSafe())
							list = list + col + (col + j) + oldPiece + promotionOptions[k] + "P";

						chessBoard[row][col] = "P";
						chessBoard[row - 1][col + j] = oldPiece;
					}
				}

			} catch (Exception e) {
			}

		}
		// Move The pawn one up
		try {
			if (" ".equals(chessBoard[row - 1][col]) && i >= 16) {
				oldPiece = chessBoard[row - 1][col];
				chessBoard[row][col] = " ";
				chessBoard[row - 1][col] = "P";
				if (isKingSafe())
					list = list + row + col + (row - 1) + col + oldPiece;
				chessBoard[row][col] = "P";
				chessBoard[row - 1][col] = oldPiece;
			}

		} catch (Exception e) {
		}

		// Move The pawn two tiles up
		try {
			if (" ".equals(chessBoard[row - 1][col]) && " ".equals(chessBoard[row - 2][col]) && i >= 48) {
				oldPiece = chessBoard[row - 2][col];
				chessBoard[row][col] = " ";
				chessBoard[row - 2][col] = "P";
				if (isKingSafe())
					list = list + row + col + (row - 2) + col + oldPiece;
				chessBoard[row][col] = "P";
				chessBoard[row - 2][col] = oldPiece;
			}

		} catch (Exception e) {
		}

		// Promotion without capture
		try {
			if (" ".equals(chessBoard[row - 1][col]) && i < 16) {
				String[] promotionOptions = { "Q", "R", "B", "K" };
				for (int k = 0; k < promotionOptions.length; k++) {
					oldPiece = chessBoard[row - 1][col];
					chessBoard[row][col] = " ";
					chessBoard[row - 1][col] = promotionOptions[k];

					if (isKingSafe())
						list = list + col + col + oldPiece + promotionOptions[k] + "P";

					chessBoard[row][col] = "P";
					chessBoard[row - 1][col] = oldPiece;
				}
			}

		} catch (Exception e) {
		}

		return list;
	}

	// This method Finds all the possible moves of Rooks
	public static String possibleR(int i) {
		String list = "";
		String oldPiece;
		int row = i / 8;
		int col = i % 8;
		int temp = 1;

		for (int j = -1; j <= 1; j += 2) {

			// check all the possible moves of a rook in it's file(column)
			try {
				while (" ".equals(chessBoard[row][col + temp * j])) {

					oldPiece = chessBoard[row][col + temp * j];
					chessBoard[row][col] = " ";
					chessBoard[row][col + temp * j] = "R";

					if (isKingSafe()) {
						list = list + row + col + row + (col + temp * j) + oldPiece;
					}
					chessBoard[row][col] = "R";
					chessBoard[row][col + temp * j] = oldPiece;
					temp++;
				}
				if (Character.isLowerCase(chessBoard[row][col + temp * j].charAt(0))) {
					oldPiece = chessBoard[row][col + temp * j];
					chessBoard[row][col] = " ";
					chessBoard[row][col + temp * j] = "R";

					if (isKingSafe()) {
						list = list + row + col + row + (col + temp * j) + oldPiece;
					}
					chessBoard[row][col] = "R";
					chessBoard[row][col + temp * j] = oldPiece;
				}

			} catch (Exception e) {
			}
			temp = 1;
			// checks all the possible moves of a Rook in it's rank(row)
			try {
				while (" ".equals(chessBoard[row + temp * j][col])) {

					oldPiece = chessBoard[row + temp * j][col];
					chessBoard[row][col] = " ";
					chessBoard[row + temp * j][col] = "R";

					if (isKingSafe()) {
						list = list + row + col + (row + temp * j) + col + oldPiece;
					}
					chessBoard[row][col] = "R";
					chessBoard[row + temp * j][col] = oldPiece;
					temp++;
				}
				if (Character.isLowerCase(chessBoard[row + temp * j][col].charAt(0))) {
					oldPiece = chessBoard[row + temp * j][col];
					chessBoard[row][col] = " ";
					chessBoard[row + temp * j][col] = "R";

					if (isKingSafe()) {
						list = list + row + col + (row + temp * j) + col + oldPiece;
					}
					chessBoard[row][col] = "R";
					chessBoard[row + temp * j][col] = oldPiece;
				}

			} catch (Exception e) {
			}
			temp = 1;
		}
		return list;
	}

	// This method Finds all the possible moves of Kinghts
	public static String possibleK(int i) {
		String list = "";
		String oldPiece;
		int row = i / 8;
		int col = i % 8;

		for (int j = -1; j <= 1; j += 2) {
			for (int k = -1; k <= 1; k += 2) {
				try {
					if (Character.isLowerCase(chessBoard[row + j][col + k * 2].charAt(0))
							|| " ".equals(chessBoard[row + j][col + k * 2])) {
						oldPiece = chessBoard[row + j][col + k * 2];
						chessBoard[row][col] = " ";

						if (isKingSafe())
							list = list + row + col + (row + j) + (col + k * 2) + oldPiece;

						chessBoard[row][col] = "K";
						chessBoard[row + j][col + k * 2] = oldPiece;
					}
				} catch (Exception e) {
				}

				try {
					if (Character.isLowerCase(chessBoard[row + j * 2][col + k].charAt(0))
							|| " ".equals(chessBoard[row + j * 2][col + k])) {
						oldPiece = chessBoard[row + j * 2][col + k];
						chessBoard[row][col] = " ";

						if (isKingSafe())
							list = list + row + col + (row + j * 2) + (col + k) + oldPiece;

						chessBoard[row][col] = "K";
						chessBoard[row + j * 2][col + k] = oldPiece;
					}
				} catch (Exception e) {
				}
			}
		}
		return list;
	}

	// This method Finds all the possible moves of Bishops
	public static String possibleB(int i) {
		String list = "";
		String oldPiece;
		int row = i / 8;
		int col = i % 8;
		int temp = 1;

		/**
		 * The logic for the bishop is same as the queen only difference is that it
		 * can't move in a streight line thats why we are incrementing both k and j with
		 * +2 so that there values are never zero only -1 and 1 because j and k are
		 * never 0 the bishop will always move diagonally
		 */
		for (int j = -1; j <= 1; j += 2) {
			for (int k = -1; k <= 1; k += 2) {

				int rowTemp = row + temp * j, colTemp = col + temp * k;
				try {
					while (" ".equals(chessBoard[rowTemp][colTemp])) {

						oldPiece = chessBoard[rowTemp][colTemp];
						chessBoard[row][col] = " ";
						chessBoard[rowTemp][colTemp] = "B";

						if (isKingSafe()) {
							list = list + row + col + rowTemp + colTemp + oldPiece;
						}

						chessBoard[row][col] = "B";
						chessBoard[rowTemp][colTemp] = oldPiece;
						temp++;
						rowTemp = row + temp * j;
						colTemp = col + temp * k;
					}

					if (Character.isLowerCase(chessBoard[rowTemp][colTemp].charAt(0))) {
						oldPiece = chessBoard[rowTemp][colTemp];
						chessBoard[row][col] = " ";
						chessBoard[rowTemp][colTemp] = "B";
						if (isKingSafe()) {
							list = list + row + col + rowTemp + colTemp + oldPiece;
						}
						chessBoard[row][col] = "B";
						chessBoard[rowTemp][colTemp] = oldPiece;
					}
				} catch (Exception e) {
				}
				temp = 1;
			}
		}
		return list;
	}

	// This method Finds all the possible moves of Queens
	public static String possibleQ(int i) {
		String list = "";
		String oldPiece;
		int row = i / 8;
		int col = i % 8;
		int temp = 1;

		/**
		 * these two nested for loop below represents the horizontal, vertical and
		 * diagonal movement of the queen
		 * 
		 * examples if outer loop and inner loops are -1 then it means to move towards
		 * upper left corner diagonally if outer loop and inner loops are 1 then it
		 * means to move towards bottom left corner diagonally if outer loop is -1 and
		 * inner is 1 then it means to move towards upper right corner diagonally if
		 * outer loop is 0 and inner is 1 or -1 then it means move right or left
		 * horizontally if outer loop is 1 or -1 and inner loop is 0 then it means move
		 * downward or upward vertically
		 * 
		 * the inner most while loop checks if the queen encounters a pice in its way
		 * because then it won't be able to move beyond that piece
		 */
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {

				int rowTemp = row + temp * j, colTemp = col + temp * k;
				if (j != 0 || k != 0) {
					try {
						while (" ".equals(chessBoard[rowTemp][colTemp])) {

							oldPiece = chessBoard[rowTemp][colTemp];
							chessBoard[row][col] = " ";
							chessBoard[rowTemp][colTemp] = "Q";

							if (isKingSafe()) {
								list = list + row + col + rowTemp + colTemp + oldPiece;
							}

							chessBoard[row][col] = "Q";
							chessBoard[rowTemp][colTemp] = oldPiece;
							temp++;
							rowTemp = row + temp * j;
							colTemp = col + temp * k;
						}

						if (Character.isLowerCase(chessBoard[rowTemp][colTemp].charAt(0))) {
							oldPiece = chessBoard[rowTemp][colTemp];
							chessBoard[row][col] = " ";
							chessBoard[rowTemp][colTemp] = "Q";
							if (isKingSafe()) {
								list = list + row + col + rowTemp + colTemp + oldPiece;
							}
							chessBoard[row][col] = "Q";
							chessBoard[rowTemp][colTemp] = oldPiece;
						}
					} catch (Exception e) {
					}
					temp = 1;
				}
			}
		}
		return list;
	}

	// This method Finds all the possible moves of Kings
	public static String possibleA(int i) {
		String list = "";
		String oldPiece;
		int row = i / 8;
		int col = i % 8;

		/**
		 * the for loop bleow iterate through all the 9 squares which a king can move if
		 * allowed including the one square on which it is curruntly at
		 */
		for (int itr = 0; itr < 9; itr++) {
			int rowTemp = row - 1 + itr / 3;
			int colTemp = col - 1 + itr % 3;
			if (itr != 4) {
				try {
					if (Character.isLowerCase(chessBoard[rowTemp][colTemp].charAt(0))
							|| " ".equals(chessBoard[rowTemp][colTemp])) {
						oldPiece = chessBoard[rowTemp][colTemp];
						chessBoard[row][col] = " ";
						chessBoard[rowTemp][colTemp] = "A";
						int kingTemp = whiteKingPosition;
						whiteKingPosition = i + (itr / 3) * 8 + itr % 3 - 9;

						if (isKingSafe()) {
							list = list + row + col + rowTemp + colTemp + oldPiece;
						}

						chessBoard[row][col] = "A";
						chessBoard[rowTemp][colTemp] = oldPiece;
						whiteKingPosition = kingTemp;
					}
				} catch (Exception e) {

				}
			}
		}
		return list;
	}

	// This method checks wheather the king is in check or not
	static boolean isKingSafe() {

		// bishop/queen
		int temp = 1;
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				try {
					while (" ".equals(chessBoard[whiteKingPosition / 8 + temp * i][whiteKingPosition % 8 + temp * j]))
						temp++;
					if ("b".equals(chessBoard[whiteKingPosition / 8 + temp * i][whiteKingPosition % 8 + temp * j])
							|| "q".equals(
									chessBoard[whiteKingPosition / 8 + temp * i][whiteKingPosition % 8 + temp * j]))
						return false;

				} catch (Exception e) {
				}
				temp = 1;
			}
		}
		// rook/queen
		for (int i = -1; i <= 1; i += 2) {
			try {
				while (" ".equals(chessBoard[whiteKingPosition / 8][whiteKingPosition % 8 + temp * i]))
					temp++;
				if ("r".equals(chessBoard[whiteKingPosition / 8][whiteKingPosition % 8 + temp * i])
						|| "q".equals(chessBoard[whiteKingPosition / 8][whiteKingPosition % 8 + temp * i]))
					return false;

			} catch (Exception e) {
			}
			temp = 1;
			try {
				while (" ".equals(chessBoard[whiteKingPosition / 8 + temp * i][whiteKingPosition % 8]))
					temp++;
				if ("r".equals(chessBoard[whiteKingPosition / 8 + temp * i][whiteKingPosition % 8])
						|| "q".equals(chessBoard[whiteKingPosition / 8 + temp * i][whiteKingPosition % 8]))
					return false;

			} catch (Exception e) {
			}
			temp = 1;
		}
		// knight
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				try {
					if ("k".equals(chessBoard[whiteKingPosition / 8 + i][whiteKingPosition % 8 + j * 2]))
						return false;

				} catch (Exception e) {
				}
				try {
					if ("k".equals(chessBoard[whiteKingPosition / 8 + i * 2][whiteKingPosition % 8 + j]))
						return false;

				} catch (Exception e) {
				}
			}
		}
		// pawn
		if (whiteKingPosition >= 16) {
			try {
				if ("p".equals(chessBoard[whiteKingPosition / 8 - 1][whiteKingPosition % 8 - 1]))
					return false;

			} catch (Exception e) {
			}
			try {
				if ("p".equals(chessBoard[whiteKingPosition / 8 - 1][whiteKingPosition % 8 + 1]))
					return false;

			} catch (Exception e) {
			}
			// king
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i != 0 || j != 0) {
						try {
							if ("a".equals(chessBoard[whiteKingPosition / 8 + i][whiteKingPosition % 8 + j]))
								return false;

						} catch (Exception e) {
						}
					}
				}
			}
		}
		return true;
	}

}
