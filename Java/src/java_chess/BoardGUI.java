package java_chess;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import javax.swing.JPanel;

public class BoardGUI extends JPanel implements MouseListener, MouseMotionListener {

	private static final long serialVersionUID = 1L;

	static int mouseClickedX, mouseClickedY;
	static int mouseReleasedX, mouseReleasedY;

	static int squareSize = 75;

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
					g.setColor(new Color(238, 239, 211, 255));// whte tiles
				else
					g.setColor(new Color(119, 150, 84, 255));// dark tiles
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
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whitePawn.png"));
					break;
				case "p":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackPawn.png"));
					break;
				case "R":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteRook.png"));
					break;
				case "r":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackRook.png"));
					break;
				case "K":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteKnight.png"));
					break;
				case "k":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackKnight.png"));
					break;
				case "B":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteBishop.png"));
					break;
				case "b":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackBishop.png"));
					break;
				case "Q":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteQueen.png"));
					break;
				case "q":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackQueen.png"));
					break;
				case "A":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteKing.png"));
					break;
				case "a":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackKing.png"));
					break;
				default:
					chessPieceImage = null;
					break;
				}

			} else {
				switch (Board.chessBoard[i / 8][i % 8]) {
				case "p":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whitePawn.png"));
					break;
				case "P":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackPawn.png"));
					break;
				case "r":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteRook.png"));
					break;
				case "R":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackRook.png"));
					break;
				case "k":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteKnight.png"));
					break;
				case "K":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackKnight.png"));
					break;
				case "b":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteBishop.png"));
					break;
				case "B":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackBishop.png"));
					break;
				case "q":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteQueen.png"));
					break;
				case "Q":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackQueen.png"));
					break;
				case "a":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/whiteKing.png"));
					break;
				case "A":
					chessPieceImage = Toolkit.getDefaultToolkit()
							.getImage(getClass().getResource("/resources/blackKing.png"));
					break;
				default:
					chessPieceImage = null;
					break;
				}

			}
//			Image scaledImage = chessPieceImage.getScaledInstance(squareSize, squareSize, Image.SCALE_SMOOTH);
//			g.drawImage(scaledImage, (i % 8) * squareSize, (i / 8) * squareSize, this);
			g.drawImage(chessPieceImage, (i % 8) * squareSize, (i / 8) * squareSize, squareSize, squareSize, this);
			repaint();
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
					repaint();
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
		e.getX();
		e.getY();
		repaint();
	}

	@Override
	public void mouseEntered(MouseEvent e) {
	}

	@Override
	public void mouseExited(MouseEvent e) {
	}
}