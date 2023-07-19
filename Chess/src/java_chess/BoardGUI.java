package java_chess;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

public class BoardGUI extends JPanel implements MouseListener, MouseMotionListener {

	private static final long serialVersionUID = 1L;

	static int mouseClickedX, mouseClickedY;
	static int mouseReleasedX, mouseReleasedY;

	static int squareSize = 70;

	BoardGUI() {
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
	}

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		this.setBackground(Color.white);

		drawBoard(g);
		drawPieces(g);

	}

	void drawBoard(Graphics g) {
		Boolean white = true;
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				if (white)
					g.setColor(Color.white);
				else
					g.setColor(Color.darkGray);
				g.fillRect(x * squareSize, y * squareSize, squareSize, squareSize);
				white = !white;
			}
			white = !white;
		}
	}

	void drawPieces(Graphics g) {
		Image chessPieceImage = null;
		for (int i = 0; i < 64; i++) {
			if (Main.humanAsWhite == 1) {
				switch (Board.chessBoard[i / 8][i % 8]) {
				case "P":
					chessPieceImage = new ImageIcon("whitePawn.png").getImage();
					break;
				case "p":
					chessPieceImage = new ImageIcon("blackPawn.png").getImage();
					break;
				case "R":
					chessPieceImage = new ImageIcon("whiteRook.png").getImage();
					break;
				case "r":
					chessPieceImage = new ImageIcon("blackRook.png").getImage();
					break;
				case "K":
					chessPieceImage = new ImageIcon("whiteKnight.png").getImage();
					break;
				case "k":
					chessPieceImage = new ImageIcon("blackKnight.png").getImage();
					break;
				case "B":
					chessPieceImage = new ImageIcon("whiteBishop.png").getImage();
					break;
				case "b":
					chessPieceImage = new ImageIcon("blackBishop.png").getImage();
					break;
				case "Q":
					chessPieceImage = new ImageIcon("whiteQueen.png").getImage();
					break;
				case "q":
					chessPieceImage = new ImageIcon("blackQueen.png").getImage();
					break;
				case "A":
					chessPieceImage = new ImageIcon("whiteKing.png").getImage();
					break;
				case "a":
					chessPieceImage = new ImageIcon("blackKing.png").getImage();
					break;
				default:
					chessPieceImage = null;
					break;
				}
				repaint();
			} else {
				switch (Board.chessBoard[i / 8][i % 8]) {
				case "P":
					chessPieceImage = new ImageIcon("blackPawn.png").getImage();
					break;
				case "p":
					chessPieceImage = new ImageIcon("whitePawn.png").getImage();
					break;
				case "R":
					chessPieceImage = new ImageIcon("blackRook.png").getImage();
					break;
				case "r":
					chessPieceImage = new ImageIcon("whiteRook.png").getImage();
					break;
				case "K":
					chessPieceImage = new ImageIcon("blackKnight.png").getImage();
					break;
				case "k":
					chessPieceImage = new ImageIcon("whiteKnight.png").getImage();
					break;
				case "B":
					chessPieceImage = new ImageIcon("blackBishop.png").getImage();
					break;
				case "b":
					chessPieceImage = new ImageIcon("whiteBishop.png").getImage();
					break;
				case "Q":
					chessPieceImage = new ImageIcon("blackQueen.png").getImage();
					break;
				case "q":
					chessPieceImage = new ImageIcon("whiteQueen.png").getImage();
					break;
				case "A":
					chessPieceImage = new ImageIcon("blackKing.png").getImage();
					break;
				case "a":
					chessPieceImage = new ImageIcon("whiteKing.png").getImage();
					break;
				default:
					chessPieceImage = null;
					break;
				}
				repaint();
			}
			g.drawImage(chessPieceImage, (i % 8) * squareSize, (i / 8) * squareSize, squareSize, squareSize, this);

		}

	}

	@Override
	public void mouseMoved(MouseEvent e) {

	}

	@Override
	public void mousePressed(MouseEvent e) {
		// if mouse is clicked inside the board
		if (e.getX() < 8 * squareSize && e.getY() < 8 * squareSize) {
			mouseClickedX = e.getX();
			mouseClickedY = e.getY();
			repaint();
//			System.out.println(mouseClickedX);
//			System.out.println(mouseClickedY);
		}
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		if (e.getX() < 8 * squareSize && e.getY() < 8 * squareSize) {
			// if inside the board
			mouseReleasedX = e.getX();
			mouseReleasedY = e.getY();
			if (e.getButton() == MouseEvent.BUTTON1) {
				String dragMove;
				if (mouseReleasedY / squareSize == 0 && mouseClickedY / squareSize == 1
						&& "P".equals(Board.chessBoard[mouseClickedY / squareSize][mouseClickedX / squareSize])) {
					// pawn promotion
					dragMove = "" + mouseClickedX / squareSize + mouseReleasedX / squareSize
							+ Board.chessBoard[mouseReleasedY / squareSize][mouseReleasedX / squareSize] + "QP";
				} else {
					// regular move
					dragMove = "" + mouseClickedY / squareSize + mouseClickedX / squareSize
							+ mouseReleasedY / squareSize + mouseReleasedX / squareSize
							+ Board.chessBoard[mouseReleasedY / squareSize][mouseReleasedX / squareSize];
				}
				String userPosibilities = Engine.posibleMoves();
				if (userPosibilities.replaceAll(dragMove, "").length() < userPosibilities.length()) {
					// if valid move
					Board.makeMove(dragMove);
					Board.flipBoard();
					Board.makeMove(Engine.alphaBeta(Engine.globalDepth, 1000000, -1000000, "", 0));
					Board.flipBoard();
					repaint();
				}
			}
		}
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		e.getX();
		e.getY();
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