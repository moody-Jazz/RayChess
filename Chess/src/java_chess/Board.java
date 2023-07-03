package java_chess;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

public class Board extends JPanel implements MouseListener, MouseMotionListener {

	private static final long serialVersionUID = 1L;

	static int x; // coordinate x
	static int y; // coordinate y

	static int squareSize = 90;

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		this.setBackground(Color.white);
		this.addMouseListener(this);
		this.addMouseMotionListener(this);

		drawBoard(g);
		drawPieces(g);

	}

	void drawBoard(Graphics g) {
		for (int i = 0; i < 64; i += 2) {
			g.setColor(new Color(245, 245, 220));
			g.fillRect((i % 8 + (i / 8) % 2) * squareSize, (i / 8) * squareSize, squareSize, squareSize);
			g.setColor(new Color(130, 102, 68));
			g.fillRect(((i + 1) % 8 - ((i + 1) / 8) % 2) * squareSize, ((i + 1) / 8) * squareSize, squareSize,
					squareSize);
		}
	}
	
	public void drawPieces(Graphics g) {
		Image chessPieceImage;
		chessPieceImage = new ImageIcon("ChessPieces.png").getImage();
		for (int i = 0; i < 64; i++) {
			int r = -1, c = -1;
			switch (Main.chessBoard[i / 8][i % 8]) {
			case "P": r = 0; c = 5; break;
			case "p": r = 1; c = 5; break;
			case "R": r = 0; c = 4; break;
			case "r": r = 1; c = 4; break;
			case "K": r = 0; c = 3; break;
			case "k": r = 1; c = 3; break;
			case "B": r = 0; c = 2; break;
			case "b": r = 1; c = 2; break;
			case "Q": r = 0; c = 1; break;
			case "q": r = 1; c = 1; break;
			case "A": r = 0; c = 0; break;
			case "a": r = 1; c = 0; break;
			}
			if (r != -1 && c != -1)
//            	g.drawImage(chessPieceImage, (i%8)*squareSize, (i/8)*squareSize, squareSize, squareSize, this);
				g.drawImage(chessPieceImage, (i % 8) * squareSize, (i / 8) * squareSize, (i % 8 + 1) * squareSize,
						(i / 8 + 1) * squareSize, c * 200, r * 200, (c + 1) * 200, (r + 1) * 200, this);
		}
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		x = e.getX();
		y = e.getY();
		repaint();
	}

	@Override
	public void mousePressed(MouseEvent e) {
	}

	@Override
	public void mouseReleased(MouseEvent e) {
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		x = e.getX();
		y = e.getY();
		repaint();
	}

	@Override
	public void mouseDragged(MouseEvent e) {
	}

	@Override
	public void mouseEntered(MouseEvent e) {
	}

	@Override
	public void mouseExited(MouseEvent e) {
	}
}