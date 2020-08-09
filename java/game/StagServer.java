import java.io.*;
import java.net.*;
import java.util.*;

class StagServer
{
    private ArrayList<Location> locationList = new ArrayList<>();
    private ArrayList<Action> actionList = new ArrayList<>();
    private ArrayList<Player> playerList = new ArrayList<>();

    public static void main(String[] args)
    {
        if(args.length != 2) System.out.println("Usage: java StagServer <entity-file> <action-file>");
        else new StagServer(args[0], args[1], 8888);
    }

    public StagServer(String entityFilename, String actionFilename, int portNumber)
    {
        DotParser dotParser = new DotParser();
        dotParser.processFile(entityFilename, locationList);

        JsonParser jsonParser = new JsonParser();
        jsonParser.processFile(actionFilename, actionList);

        try {
            ServerSocket ss = new ServerSocket(portNumber);
            System.out.println("Server Listening");
            while(true) acceptNextConnection(ss);
        } catch(IOException ioe) {
            System.err.println(ioe);
        }
    }

    private void acceptNextConnection(ServerSocket ss)
    {
        try {
            // Next line will block until a connection is received
            Socket socket = ss.accept();
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            processNextCommand(in, out);
            out.close();
            in.close();
            socket.close();
        } catch(IOException ioe) {
            System.err.println(ioe);
        }
    }

    private void processNextCommand(BufferedReader in, BufferedWriter out) throws IOException
    {
        String line = in.readLine();
        // Obtain the player name from the readLine
        String playerName = line.substring(0, line.indexOf(':'));
        // Added 2 to the index of ':' to obtain the command after the ':' and space character.
        String currentCommand = line.substring(line.indexOf(':') + 2);
        // create and add player if they do not exist.
        if(!playerList.contains(Player.getPlayerFromName(playerName, playerList))){
            Player player = new Player(playerName, "Player", locationList.get(0));
            playerList.add(player);
        }
        out.write("You said... " + currentCommand + "\n");
        Commands commandHandler = new Commands();
        Player currentPlayer = Player.getPlayerFromName(playerName, playerList);
        if(currentPlayer == null){
            System.err.println("Player does not exist");
            return;
        }
        out.write(commandHandler.runCommand(currentCommand, currentPlayer, locationList, actionList));
        if(currentPlayer.getHealth() <= 0){
            out.write("\nYou have died! All your items are lost and you have returned to the start.\n");
            out.write("--------------------------------------------------------------------------\n");
            resetPlayer(currentPlayer, locationList);
            out.write("You are in the " + locationList.get(0).getName() + ".\n");
        }
    }

    void resetPlayer(Player player, ArrayList<Location> locationList){
        player.clearInventory();
        Location diedLocation = Location.getLocationFromName(player.getLocation(), locationList);
        if(diedLocation == null){
            System.err.println("Location of death not recognised.");
            return;
        }
        diedLocation.removeEntity(player);
        locationList.get(0).addEntity(player);
        player.setLocation(locationList.get(0).getName());
        player.setHealth(3);
    }
}
