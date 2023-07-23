package java_chess;

public class Engine {

	static int globalDepth =3;
	
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
	
	public static String sortMoves(String list) {
		int[] score = new int[list.length() / 5];
		for (int i = 0; i < list.length(); i += 5) {
			Board.makeMove(list.substring(i, i + 5));
			score[i / 5] = -Rating.rating(-1, 0);
			Board.undoMove(list.substring(i, i + 5));
		}
		String newListA = "", newListB = list;
		for (int i = 0; i < Math.min(6, list.length() / 5); i++) {// first few moves only
			int max = -1000000, maxLocation = 0;
			for (int j = 0; j < list.length() / 5; j++) {
				if (score[j] > max) {
					max = score[j];
					maxLocation = j;
				}
			}
			score[maxLocation] = -1000000;
			newListA += list.substring(maxLocation * 5, maxLocation * 5 + 5);
			newListB = newListB.replace(list.substring(maxLocation * 5, maxLocation * 5 + 5), "");
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
