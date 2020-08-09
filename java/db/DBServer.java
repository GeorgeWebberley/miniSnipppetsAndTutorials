import java.io.*;
import java.net.*;

class DBServer
{
    final static char EOT = 4;

    public static void main(String[] args)
    {
        new DBServer(8888);
    }

    public DBServer(int portNumber)
    {
        try {
            ServerSocket ss = new ServerSocket(portNumber);
            System.out.println("Server Listening on port " + portNumber);
            while (true) clientConnection(ss);
        } catch(IOException ioe) {
            System.err.println(ioe.getMessage());
        }
    }

    private void clientConnection(ServerSocket ss)
    {
        try {
            Socket socket = ss.accept();
            System.out.println("Client connected");
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            Session session = new Session(out);
            String input = in.readLine();
            while(input != null){
                processNextCommand(input, out, session);
                input = in.readLine();
            }
        } catch(IOException ioe) {
            System.err.println(ioe.getMessage());
        }
    }

    private void processNextCommand(String input, PrintWriter out, Session session)
    {
        input = input.trim();
        // Check the command ends with a semicolon
        if(input.charAt(input.length()-1) != ';'){
            out.println("Expected ';' at the end of the query.");
        } else{
            // Remove the semicolon before parsing
            input = input.substring(0, input.length()-1);
            Query query = new Query(input, session);
            if(query.getInput().size() <= 1) out.println("Command not recognised");
            else query.execute();
        }
        out.println("" + EOT + "");
    }




}
