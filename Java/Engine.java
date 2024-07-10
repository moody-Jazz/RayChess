package java_chess;

public class Engine {

	static int globalDepth = 3;

	public static String posibleMoves() {
		String list = "";
		for (int i = 0; i < 64; i++) {
			switch (Board.chessBoard[i / 8][i % 8]) {
			case "P":
				list += PieceMovement.posibleP(i);
				break;
			case "R":
				list += PieceMovement.posibleR(i);
				break;
			case "K":
				list += PieceMovement.posibleK(i);
				break;
			case "B":
				list += PieceMovement.posibleB(i);
				break;
			case "Q":
				list += PieceMovement.posibleQ(i);
				break;
			case "A":
				list += PieceMovement.posibleA(i);
				break;
			}
		}
		return list;// x1,y1,x2,y2,captured piece
	}

	// this sortMoves exist so that we can sort the moves in order of best to worst
	// so that we can neglect the moves at the last which will make the engine faster
	public static String sortMoves(String list) {
		/*
		 * the for loop below is used to find the rating of each move to give it a score
		 * and then storing that score at the same position in score array as the move
		 * corrospoding to that score is in the list string
		 * 
		 * also we are using -1 in the rating argument becuase the in the rating method
		 * listLenght is used as argument to call the rateMovability method
		 * rateMovability is used to give rating according to the total valid move
		 * current side has but here we are only calculating score of individual move
		 * thats why are not passing something greater than zero because we only want to
		 * calculate the score of one move but we also cant pass 0 because then that
		 * would mean that current side doesn't have any move and the game is over try
		 * to read the rateMovability method in the Rating class for more understanding
		 */
		int[] score = new int[list.length() / 5];
		for (int i = 0; i < list.length(); i += 5) {
			Board.makeMove(list.substring(i, i + 5));
			score[i / 5] = -Rating.rating(-1, 0);
			Board.undoMove(list.substring(i, i + 5));
		}
		String newListA = "", newListB = list;
		/*
		 * the for loop below has the condition Math.min(6, list.length() / 5 becuase
		 * initially we just want to store the 6 best moves from the list but what if
		 * list doesn't even have 6 moves in total, then if we use 6 it will throw
		 * exception: array out of bound that is why we are using minimum of 6 or list
		 * length/5
		 * 
		 * then inside the inner loop
		 */
		for (int i = 0; i < Math.min(6, list.length() / 5); i++) {
			int max = -1000000, maxLocation = 0;
			for (int j = 0; j < score.length; j++) {
				if (score[j] > max) {
					max = score[j];
					maxLocation = j;
				}
			}
			score[maxLocation] = -1000000;
			newListA += list.substring(maxLocation * 5, maxLocation * 5 + 5);// adding best in newListA
			newListB = newListB.replace(list.substring(maxLocation * 5, maxLocation * 5 + 5), "");// remove best
		}
		return newListA + newListB;
	}

	public static String alphaBeta(int depth, int beta, int alpha, String move, int player) {
		// return in the form of 1234b##########
		String list = posibleMoves();
		if (depth == 0 || list.length() == 0) {
			return move + (Rating.rating(list.length(), depth) * (player * 2 - 1));
		}
		list = sortMoves(list);
		player = 1 - player;// either 1 or 0
		for (int i = 0; i < list.length(); i += 5) {
			Board.makeMove(list.substring(i, i + 5));
			Board.flipBoard();
			String returnString = alphaBeta(depth - 1, beta, alpha, list.substring(i, i + 5), player);
			int value = Integer.valueOf(returnString.substring(5));
			Board.flipBoard();
			Board.undoMove(list.substring(i, i + 5));
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
				if (player == 0) {
					return move + beta;
				} else {
					return move + alpha;
				}
			}
		}
		if (player == 0) {
			return move + beta;
		} else {
			return move + alpha;
		}
	}
}
