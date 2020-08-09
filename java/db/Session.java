import java.io.*;
import java.util.ArrayList;

public class Session {
    private ArrayList <Database> databases;
    private Database currentDatabase;
    private PrintWriter out;
    private FileManager fileManager = new FileManager();


    Session(PrintWriter out){
        this.databases = new ArrayList<>();
        this.currentDatabase = null;
        this.out = out;
        fileManager.readInDatabase(this);
    }


    ArrayList<Database> getDatabases(){
        return this.databases;
    }

    Database getCurrentDatabase(){
        if(currentDatabase == null) send("Please 'USE' a database first.");
        return currentDatabase;
    }

    FileManager getFileManager(){
        return this.fileManager;
    }

    void createDatabase(String name){
        for(Database d : databases){
            if(d.getName().equals(name)){
                send("Database '" + name + "' already exists.");
                return;
            }
        }
        fileManager.createDatabase(name);
        databases.add(new Database(name, this));
        send("Database '" + name + "' created successfully.");
    }

    boolean useDatabase(String name){
        for(Database d : databases){
            if(d.getName().equals(name)){
                currentDatabase = d;
                return true;
            }
        }
        // If it reaches the end of the for loop, then that database doesn't exist.
        return false;
    }

    void dropDatabase(String name){
        if(currentDatabase != null && name.equals(currentDatabase.getName())){
            currentDatabase = null;
        }
        for(Database d : databases){
           if(d.getName().equals(name)){
                databases.remove(d);
                fileManager.deleteDatabase(name);
                send("Database dropped.");
                return;
            }
        }
        // If it reaches the end of the for loop, then that database doesn't exist.
        send("Couldn't find database with name " + name);
    }

    void send(String message){
        out.println(message);
    }


}
