package java_chess;

import java.util.*;
import javax.swing.*;

public class Main {
	
	
	
	static int humanAsWhite = -1;// 1=human as white, 0=human as black
	

	public static void main(String[] args) {
		while (!"A".equals(Board.chessBoard[Board.whiteKingPosition / 8][Board.whiteKingPosition % 8])) {
			Board.whiteKingPosition++;
		} // get King's location
		while (!"a".equals(Board.chessBoard[Board.blackKingPosition / 8][Board.blackKingPosition % 8])) {
			Board.blackKingPosition++;
		} // get king's location
		/*
		 * PIECE=WHITE/black pawn=P/p kinght (horse)=K/k bishop=B/b rook (castle)=R/r
		 * Queen=Q/q King=A/a
		 * 
		 * My strategy is to create an alpha-beta tree diagram wich returns the best
		 * outcome
		 * 
		 * (1234b represents row1,column2 moves to row3, column4 which captured b (a
		 * space represents no capture))
		 */
		JFrame f = new JFrame("Chess Tutorial");
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		BoardGUI ui = new BoardGUI();
		f.add(ui);
		f.setSize(1000, 1000);
		f.setVisible(true);
	
		Object[] option = { "Computer", "Human" };
		humanAsWhite = JOptionPane.showOptionDialog(null, "Who should play as white?", "ABC Options",
				JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, option, option[1]);
		
		if (humanAsWhite == 0) {
			Board.makeMove(Engine.alphaBeta(Engine.globalDepth, 1000000, -1000000, "", 0));
			Board.flipBoard();
			f.repaint();
		}
		System.out.println(Engine.posibleMoves());
//
//		for (int i = 0; i < 8; i++) {
//			System.out.println(Arrays.toString(Board.chessBoard[i]));
//		}
	}


}