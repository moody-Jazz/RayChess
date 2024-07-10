package java_chess;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.io.IOException;

import javax.swing.*;

public class Main {

	static int humanAsWhite = -1;// 1=human as white, 0=human as black

	public static void main(String[] args) {

		JFrame f = new JFrame("Chess Tutorial");
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		BoardGUI ui = new BoardGUI();
		f.add(ui);
		Dimension ss = Toolkit.getDefaultToolkit().getScreenSize();
		Dimension frameSize = new Dimension(610, 635);
		f.setBounds(ss.width / 2 - frameSize.width / 2, ss.height / 2 - frameSize.height / 2, frameSize.width,
				frameSize.height);
		f.setVisible(true);

		Object[] option = { "Computer", "Human" };
		humanAsWhite = JOptionPane.showOptionDialog(null, "Who should play as white?", "ABC Options",
				JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, option, option[1]);

		if (humanAsWhite == 0) {
			Board.makeMove(Engine.alphaBeta(Engine.globalDepth, 1000000, -1000000, "", 0));
			Board.flipBoard();
			f.repaint();
		}
//		System.out.println(Engine.posibleMoves());
		/*
		 * // for (int i = 0; i < 8; i++) { //
		 * System.out.println(Arrays.toString(Board.chessBoard[i])); // }
		 */
	}
}