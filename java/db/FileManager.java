import java.io.*;
import java.util.ArrayList;

public class FileManager {
    // Initialise some string variables to make building the path clearer
    final String databaseSystemPath = ".."+ File.separator + "database";
    // A database prefix is used so that we don't try to read any other possible folders
    final String databasePrefix = "db_";
    final String fileExt = ".txt";


    // Create a database folder using the name of the database
    void createDatabase(String name){
        String databaseName = databaseSystemPath + File.separator + databasePrefix + name;
        File databaseFile = new File(databaseName);
        // Using mkdirs will also create the parent directory if it has not yet been made
        databaseFile.mkdirs();
    }

    // Delete a database using the name as the identifier
    void deleteDatabase(String name){
        String databaseName = databaseSystemPath + File.separator + databasePrefix + name;
        File databaseFile = new File(databaseName);
        // Get a list of all files or 'tables' in the database
        String[] tables = databaseFile.list();
        if(tables != null){
            // Loop through the list, deleting each file/table
            for(String table : tables){
                File currentFile = new File(databaseName, table);
                currentFile.delete();
            }
        }
        // Finally delete the database
        databaseFile.delete();
    }


    // Manage tables includes inserting a row, deleting a row, altering data
    public void manageTables(String databaseName, Table table){
        // We have already checked when parsing that the database exists
        String databasePath = databaseSystemPath + File.separator + databasePrefix + databaseName;
        String newTableName = databasePath + File.separator  + table.getName() + fileExt;
        File newTable = new File(newTableName);
        try {
            if (!newTable.exists()) {
                // Create table if it doesn't yet exist.
                newTable.createNewFile();
            }
            FileWriter writer = new FileWriter(newTable);
            // Get the contents of the table from the table object and write it to the file
            writer.write(tableContents(table));
            writer.flush();
            writer.close();
        }catch (IOException e){
            System.out.println(e.getMessage());
        }

    }

    // Get the contents of the table object so we can write it to a file
    private String tableContents(Table tableObject){
        ArrayList<ArrayList<String>> table = tableObject.getTableArray();
        StringBuilder tableString = new StringBuilder();
        // Loop through each row and each cell, adding a tab between each word
        for(ArrayList<String> row : table){
            for(String cell : row){
                tableString.append(cell).append('\t');
            }
            // Create new line for the next row
            tableString.append('\n');
        }
        return tableString.toString();
    }


    public boolean deleteTable(String databaseName, String tableName){
        String databasePath = databaseSystemPath + File.separator + databasePrefix + databaseName;
        String tableToDelete = databasePath + File.separator + tableName + fileExt;
        File tableFile = new File(tableToDelete);
        return tableFile.delete();
    }


    // Loop through all the database files, adding them (and the tables) to java objects to make queries faster
    public void readInDatabase(Session session){
        File databaseSystem = new File(databaseSystemPath);
        // Gets list of all databases
        String[] databases = databaseSystem.list();
        if(databases == null) return;
        // Loop through each database, checking it starts with '_db' (index 0 to 3)
        for(String databaseName : databases){
            if(databaseName.substring(0,3).equals(databasePrefix)){
                // Get the database name (without the prefix)
                Database database = new Database(databaseName.substring(3), session);
                String databasePath = databaseSystemPath + File.separator + databaseName;
                File databaseFile = new File(databasePath);
                // Get the list of tables in the database and send it to a function to read and add to the database.
                String[] tables = databaseFile.list();
                readInTables(tables, database, session);
                // Finally add the database to the current session.
                session.getDatabases().add(database);
            }
        }
    }

    // Loops through all the tables, checking they have the correct extension and then populating a table object
    private void readInTables(String[] tables, Database database, Session session){
        if(tables == null) return;
        String databasePath = databaseSystemPath + File.separator + databasePrefix + database.getName();
        // Loop through all the tables
        for(String tableFileName : tables){
            int index = tableFileName.lastIndexOf('.');
            String extension = tableFileName.substring(index);
            // Check each file has the extension '.txt'
            if(extension.equals(fileExt)){
                File tableFile = new File(databasePath, tableFileName);
                ArrayList<ArrayList<String>> readTable = readInTable(tableFile);
                // Make a substring (removing the extension)
                String tableName = tableFileName.substring(0, index);
                int numberOfAttributes = readTable.get(0).size() - 1;
                int numberOfRows = readTable.size();
                Table table = new Table(tableName, readTable, numberOfAttributes, numberOfRows, session);
                database.getTables().add(table);
            }
        }
    }


    // Reads in one file, storing the contents in a 2d arraylist
    private ArrayList<ArrayList<String>> readInTable(File tableFile){
        ArrayList<ArrayList<String>> table = new ArrayList<>();
        try{
            FileReader reader = new FileReader(tableFile);
            BufferedReader bReader = new BufferedReader(reader);
            String rowString = bReader.readLine();
            while(rowString != null){
                table.add(parseRow(rowString));
                rowString = bReader.readLine();
            }
        } catch (IOException e){
            System.err.println(e.getMessage());
        }
        return table;
    }

    // Parses a row in the text file, splitting on tab and saving each individual entry to an arraylist.
    private ArrayList<String> parseRow(String rowString){
        ArrayList<String> row = new ArrayList<>();
        // Split on tab as this is how the table was built
        String[] tempRow = rowString.split("\t");
        // Loop through each word or 'cell' in the row
        for(String cell : tempRow){
            // Check it isn't an empty string
            if(!cell.equals("")) row.add(cell);
        }
        return row;
    }




}
