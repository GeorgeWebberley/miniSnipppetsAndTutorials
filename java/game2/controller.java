import java.util.regex.Matcher;
import java.util.regex.Pattern;

class OXOController
{
    private OXOModel game;
    private int numberOfRows;
    private int numberOfColumns;
    private int numberOfPlayers;
    private int winThreshold;
    private int turnCount;
    private boolean gameFinished;


    public OXOController(OXOModel model)
    {
        game = model;
        gameFinished = false;
        numberOfPlayers = game.getNumberOfPlayers();
        numberOfRows = game.getNumberOfRows();
        numberOfColumns = game.getNumberOfColumns();
        winThreshold = game.getWinThreshold();
        turnCount = 0;
        game.setCurrentPlayer(game.getPlayerByNumber(turnCount));
    }


    public void handleIncomingCommand(String command) throws InvalidCellIdentifierException, CellAlreadyTakenException, CellDoesNotExistException
    {
        if(gameFinished) return;
        if(numberOfRows > 9 || numberOfColumns > 9){
            System.out.println("Board too large. Maximum size is 9x9.");
            return;
        }

        Pattern commandPattern = Pattern.compile("[a-zA-Z](\\d+)");
        Matcher commandMatch = commandPattern.matcher(command);

        if(!commandMatch.matches()){
            throw new InvalidCellIdentifierException("command", command);
        }

        if(commandMatch.group(1).charAt(0) == '0'){
            throw new InvalidCellIdentifierException("command", command);
        }

        // Convert the number string (column) to an integer.
        int columnNumber =  Integer.parseInt(commandMatch.group(1));
        // Subtract 1 (as column 1 represents array index 0)
        columnNumber--;

        // Convert the coordinate letter to array index.
        int rowNumber = Character.toUpperCase(command.charAt(0)) - 'A';

        // Handling the first (y) coordinate.
        if(Character.toUpperCase(command.charAt(0)) >= 'A' + numberOfRows){
            throw new CellDoesNotExistException(Character.toUpperCase(command.charAt(0)) - 'A', command.charAt(1) - '1');
        }
        // Handling the second (x) coordinate.
        if(columnNumber >= numberOfColumns){
            throw new CellDoesNotExistException(rowNumber, columnNumber);
        }
        // Cell taken??
        if(game.getCellOwner(rowNumber, columnNumber) != null){
            throw new CellAlreadyTakenException(rowNumber, columnNumber);
        }
        // If the above 'if' statements passed then we can set the value to the current player's letter.
        game.setCellOwner(rowNumber, columnNumber, game.getCurrentPlayer());

        // Checks to see if the most recent move has resulted in a victory.
        if(checkWin(rowNumber, columnNumber)){
            game.setWinner(game.getCurrentPlayer());
            gameFinished = true;
        }

        turnCount++;

        // Else if the board is full, no more moves are possible.
        if(turnCount == numberOfRows * numberOfColumns){
            game.setGameDrawn();
            gameFinished = true;
        }

        // Update player turn to next player before the next command.
        game.setCurrentPlayer(game.getPlayerByNumber(turnCount % numberOfPlayers));
    }

    public boolean checkWin(int row, int column){
        if(checkVertical(column)) return true;
        if(checkHorizontal(row)) return true;
        if(checkDiagLeft(row, column)) return true;
        if(checkDiagRight(row, column)) return true;
        return false;
    }

    public boolean checkVertical(int column){
        int rowCount = 0;
        int letterCount = 0;
        // run along the vertical, counting adjacent cells with the same letter.
        while(rowCount < numberOfRows){
            if(game.getCellOwner(rowCount, column) == game.getCurrentPlayer()){
                letterCount++;
            }else{
                letterCount = 0;
            }
            if (letterCount == winThreshold){
                return true;
            }
            rowCount++;
        }
        return false;
    }

    public boolean checkHorizontal(int row){
        int colCount = 0;
        int letterCount = 0;
        // run along the horizontal, counting adjacent cells with the same letter.
        while(colCount < numberOfColumns){
            if(game.getCellOwner(row, colCount) == game.getCurrentPlayer()){
                letterCount++;
            }else{
                letterCount = 0;
            }
            if (letterCount == winThreshold){
                return true;
            }
            colCount++;
        }
        return false;
    }

    public boolean checkDiagLeft(int row, int column){
        int letterCount = 0;
        // find the upper right most cell on the same diagonal.
        while(row > 0 && column < numberOfColumns-1){
            row--;
            column++;
        }
        // run along diagonal, counting adjacent cells with the same letter
        while(row < numberOfRows && column >= 0){
            if(game.getCellOwner(row, column) == game.getCurrentPlayer()){
                letterCount++;
            }else{
                letterCount = 0;
            }
            if (letterCount == winThreshold){
                return true;
            }
            row++;
            column--;
        }
        return false;
    }

    public boolean checkDiagRight(int row, int column){
        int letterCount = 0;
        // find the upper left most cell on the same diagonal.
        while(row > 0 && column > 0){
            row--;
            column--;
        }
        // run along diagonal, counting adjacent cells with the same letter
        while(row < numberOfRows && column < numberOfColumns){
            if(game.getCellOwner(row, column) == game.getCurrentPlayer()){
                letterCount++;
            }else{
                letterCount = 0;
            }
            if (letterCount == winThreshold){
                return true;
            }
            row++;
            column++;
        }
        return false;
    }
}
