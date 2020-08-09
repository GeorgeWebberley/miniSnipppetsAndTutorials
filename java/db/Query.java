import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class Query {
    private ArrayList<String> input = new ArrayList<>();
    private Session session;

//    Each token is either a single word, a string inside quotations or a condition.
    public Query(String input, Session session){
        this.session = session;
        StringBuilder token=new StringBuilder();
//       insideAString keeps track if we are inside a string or not.
        boolean insideAString = false;
//       Loop through each character of the input
        for(int i = 0; i < input.length(); i++){
//           As we enter a string, insideAString becomes true. As we leave, it becomes false.
            if (input.charAt(i)=='\'') insideAString = !insideAString;
            if (input.charAt(i)==' ' && !insideAString) {
                this.input.add(token.toString());
                if(token.toString().equalsIgnoreCase("WHERE")){
                    // We save everything after 'where' (i.e. the condition) as a single token.
                    String condition = input.substring(i+1);
                    this.input.add(condition);
                    return;
                }
                token.setLength(0);
            }
            else
                token.append(input.charAt(i));
        }
        this.input.add(token.toString());
    }



    public ArrayList<String> getInput(){
        return this.input;
    }


    void execute(){
        switch(firstWord().toUpperCase()){
            case "USE":
                useCommand();
                break;
            case "CREATE":
                createCommand();
                break;
            case "DROP":
                dropCommand();
                break;
            case "ALTER":
                alterCommand();
                break;
            case "INSERT":
                insertCommand();
                break;
            case "SELECT":
                selectCommand();
                break;
            case "UPDATE":
                updateCommand();
                break;
            case "DELETE":
                deleteCommand();
                break;
            case "JOIN":
                joinCommand();
                break;
            default:
                session.send("Query not recognised");
        }
    }


    private void useCommand(){
        // First argument is 'use', second argument should be database name.
        if(input.size() != 2) {
            session.send("Expected database name following 'USE' command.");
            return;
        }
        // Returns false if unable to find the database with that name.
        if(!session.useDatabase(secondWord())){
            session.send("Could not find database with name " + secondWord());
            return;
        }
        // we have passed the 2 statements and we can send a success message.
        session.send("You have selected '" + secondWord() + "'");
    }


    private void createCommand(){
        // We have already checked the input is greater than two words long in DBServer class.
        // Create a database
        if(secondWord().equalsIgnoreCase("DATABASE") && (input.size() == 3)){
            session.createDatabase(thirdWord());
            return;
        }
        // Create a table
        if(secondWord().equalsIgnoreCase("TABLE")){
            Database currentDb = session.getCurrentDatabase();
            if(currentDb == null) return;
            parseCreateTable(currentDb);
            return;
        }
        session.send("Expected 'DATABASE'/'TABLE' as second word in create statement.");
    }


    private void parseCreateTable(Database currentDb){
        String tableName = thirdWord();
        // Check not reading out of bounds.
        if(tableName == null){
            session.send("Expected name following 'CREATE TABLE'");
            return;
        }
        // Check if table already exists.
        for(Table table: currentDb.getTables()){
            if(table.getName().equals(tableName)){
                session.send(tableName + " already exists.");
                return;
            }
        }
        ArrayList<String> attributes = new ArrayList<>();
        //  Check if there is an attribute list
        if(input.size() > 3) {
            // create a sub array which should be the attributes, with brackets and commas included.
            // Starts at index 3 (following CREATE TABLE <name>).
            attributes = new ArrayList<>(input.subList(3, input.size()));
        }
        if(tryTokeniseList(attributes)){
            Table table = currentDb.createTable(tableName, attributes);
            FileManager fm = session.getFileManager();
            fm.manageTables(currentDb.getName(), table);
            session.send("Table created successfully.");
        }
    }



    private void dropCommand(){
        // If the user wishes to drop a database, the command should be 3 words long (the last being the name)
        if(input.size() != 3){
            session.send("Error: drop command has incorrect number of arguments.");
            return;
        }
        // Second word is the structure, third word is the name.
        if(secondWord().equalsIgnoreCase("DATABASE")){
            session.dropDatabase(thirdWord());
        }
        if(secondWord().equalsIgnoreCase("TABLE")){
            Database currentDb = session.getCurrentDatabase();
            if (currentDb == null) return;
            // the third word will be the table name;
            Table table = currentDb.getTable(thirdWord());
            if(table == null) return;
            currentDb.dropTable(table);
            FileManager fm = session.getFileManager();
            boolean result = fm.deleteTable(currentDb.getName(), thirdWord());

            if(result) session.send("Table dropped successfully.");
        }
    }


    private void alterCommand(){
        Database currentDb = session.getCurrentDatabase();
        if(currentDb == null) return;
        String tableName = thirdWord();
        // Alteration type is add or drop
        String alterationType = fourthWord();
        String attributeName = fifthWord();
        if(!secondWord().equalsIgnoreCase("Table")){
            session.send("Expected 'TABLE' as second word in alter command.");
            return;
        }
        Table table = currentDb.getTable(tableName);
        if(table != null && alterationType != null && attributeName != null){
            table.addOrDropAttribute(alterationType, attributeName);
            FileManager fm = session.getFileManager();
            fm.manageTables(currentDb.getName(), table);
            session.send("Table altered successfully.");
        } else{
            session.send("Incorrect number of arguments given to ALTER command.");
            session.send("Correct usage: ALTER TABLE <TableName> ADD/DROP <AttributeName>");
        }
    }

    private void insertCommand(){
        // Check user has used a database first.
        Database currentDb = session.getCurrentDatabase();
        if(currentDb == null) return;
        // An insert command has to be at least 5 words long.
        // The second word should be 'INTO' and the fourth word 'VALUES'
        if(input.size() < 5 || !secondWord().equalsIgnoreCase("INTO")){
            session.send("Error: correct usage is 'INSERT INTO <table> VALUES <values>");
            return;
        }
        if(!fourthWord().equalsIgnoreCase("VALUES")){
            session.send("Error: Expected 'VALUES' as fourth word in insert statement.");
            return;
        }
        // get the table name and table object
        String tableName = thirdWord();
        if(tableName == null) return;
        Table table = currentDb.getTable(tableName);
        // Separate the value list from the rest of the command (From the 4th index to the end).
        ArrayList<String> valueList = new ArrayList<>(input.subList(4, input.size()));

        // Check we have valid table and that it is possible to tokenise the value list.
        if(table == null || !tryTokeniseList(valueList)) return;
        if(table.insertValues(valueList)){
            session.send("Data inserted successfully");
            session.getFileManager().manageTables(currentDb.getName(), table);
        }else{
            session.send("Incorrect number of values given.");
        }
    }

    // This function parses the select command, checking the correct words are present.
    private void selectCommand(){
        Database currentDb = session.getCurrentDatabase();
        if(currentDb == null) return;
        int fromIndex = indexOfWord("FROM");
        // Check that 'FROM' is not in the first two words.
        // Also check that the query is long enough to contain the table name (1 word beyond the from index).
        if(fromIndex > 1 && input.size() > fromIndex + 1){
            // Make a subarray of attributes.
            // The attributes will be between 'SELECT' (index 0) and 'FROM' (fromIndex)
            ArrayList<String> attributes = new ArrayList<>(input.subList(1, fromIndex));
            // Remove the commas if more than 1 attribute.
            if(!tryRemoveCommas(attributes)) return;
            // the index of the table name will be 1 more than the index of 'FROM'
            String tableName = input.get(fromIndex + 1);
            Table table = currentDb.getTable(tableName);
            if(table == null) return;
//          the whereIndex should be 2 words past the fromIndex
            int whereIndex = fromIndex + 2;
            selectRowsAndCols(attributes, whereIndex, table);
        } else if (fromIndex == 1){
            session.send("Expected attributes in select statement before 'FROM'");
        }
    }

    // This method selects all the rows and columns needed in a select query.
    void selectRowsAndCols(ArrayList<String> attributes, int whereIndex, Table table){
        // Extract the 2d arraylist from the table object.
        ArrayList<ArrayList<String>> tempTable = table.getTableArray();
        ArrayList<ArrayList<String>> finalTable;
        // If the next word is 'WHERE', we first filter the table rows based on the condition.
        if(whereIndex < input.size() && input.get(whereIndex).toUpperCase().equals("WHERE")){
            // A condition is always 1 token beyond whereIndex
            int conditionIndex = whereIndex + 1;
            List<Integer> rowIndices = getRowsUsingConds(table, conditionIndex);
            if(rowIndices == null) return;
            // Create a 2d array sub-table based on the results of the 'WHERE' conditions
            tempTable = table.filterTableRows(rowIndices);
        } else if (whereIndex < input.size()){
            session.send("Missing 'WHERE' before conditional statement.");
            return;
        }

        // Now we can filter the table columns based on the selected attributes.
        if(attributes.size() == 1 && attributes.get(0).equals("*")){
            // If the attribute is '*' then we don't need to filter.
            finalTable = tempTable;
        } else{
            List<Integer> indexList = table.getColumnIndices(attributes);
            if(indexList == null) return;
            // Create a sub-table, based on the select parameters in the query
            finalTable = table.filterTableColumns(indexList, tempTable);
        }
        if(finalTable != null) table.printTable(finalTable);
    }

    List<Integer> getRowsUsingConds(Table table, int conditionIndex){
        if(conditionIndex >= input.size()){
            session.send("Expected conditional statement after 'WHERE'");
            return null;
        }
        String condition = input.get(conditionIndex);
        // Filter our all rows which match the conditions
        return table.getRowIndices(condition);
    }



    //    Created my own function here, so that it can be case insensitive
    private int indexOfWord(String word){
        // We can skip the first word as we know this is 'SELECT'
        for(int i = 1; i < input.size(); i++){
            if(input.get(i).equalsIgnoreCase(word)){
                return i;
            }
        }
        session.send("Missing " + word + " statement in " + firstWord() + " query.");
        return -1;
    }



    private void updateCommand(){
        int whereIndex = indexOfWord("WHERE");
        if (whereIndex == -1) return;
        // Check the third word is 'SET' (and inbounds)
        if(input.size() >= 3 && thirdWord().equalsIgnoreCase("SET")){
            // We can create a substring for name-value pairs (these will be between 'SET' (index 2) and the whereIndex)
            ArrayList<String> nameValues = new ArrayList<>(input.subList(3, whereIndex));
            // Pass the string to a function which inserts the name value pairs into a hashmap.
            HashMap <String, String> nameValuePairs = getNameValuePairs(nameValues);
            // THIS EXACT CODE BELOW IS ALSO USED IN SELECTCOMMAND
            Database currentDb = session.getCurrentDatabase();
            if(nameValuePairs == null || currentDb == null) return;
            Table table = currentDb.getTable(secondWord());
            if(table == null) return;
            // A condition should always be 1 token after 'WHERE'
            int conditionIndex = whereIndex + 1;
            List<Integer> rowIndices = getRowsUsingConds(table, conditionIndex);
            if(rowIndices == null) return;
            table.updateTable(rowIndices, nameValuePairs);
            session.send("Table updated successfully.");
            session.getFileManager().manageTables(currentDb.getName(), table);
        } else{
            session.send("Expected 'SET' as third word in 'UPDATE' statement (After table name).");
        }
    }


    private HashMap<String, String> getNameValuePairs(ArrayList<String> nameValues){
        HashMap <String, String> nameValuePairs = new HashMap<>();
        int index = 0;
        while(index < nameValues.size()){
            // name-value pairs are 3 tokens long (attribute = value)
            // therefore we can add 2 to check we wont go out of bounds
            if(index + 2 >= nameValues.size()){
                session.send("name-value declaration too short");
                session.send("Correct usage: <AttributeName> = <Value>");
                return null;
            }
            String attribute = nameValues.get(index);
            index++;
            // Check the second token is equal to '='
            if(!nameValues.get(index).equals("=")){
                session.send("Expected '=' in name/value pair after the attribute.");
                return null;
            }
            index++;
            String value = nameValues.get(index);
            index++;
            // If we haven't reached the end of nameValues then the value should end with a comma
            if(index < nameValues.size() && value.charAt(value.length()-1) == ','){
                // Remove the comma
                value = value.substring(0, value.length()-1);
            } else if(index < nameValues.size()){
                session.send("Comma needed between each name-value pair.");
                return null;
            }
            // Finally, save the name-value pair in a hashmap.
            nameValuePairs.put(attribute, value);
        }
        return nameValuePairs;
    }


    private void deleteCommand(){
        if(!secondWord().equalsIgnoreCase("FROM")){
            session.send("Expected 'FROM' after 'DELETE'");
            return;
        }
        if(!fourthWord().equalsIgnoreCase("WHERE")){
            session.send("Expected 'WHERE' after table name.");
            return;
        }
        Database currentDb = session.getCurrentDatabase();
        if (currentDb == null) return;
        Table table = currentDb.getTable(thirdWord());
        if(table == null) return;
        // In a delete command, the condition should always be at the 4th index;
        List<Integer> rowIndices = getRowsUsingConds(table, 4);
        if (rowIndices == null) return;
        table.deleteRows(rowIndices);
        session.getFileManager().manageTables(currentDb.getName(), table);
        session.send("Data deleted.");
    }


    private void joinCommand(){
//      A join command is 8 tokens long
        if(input.size() != 8){
            session.send("Correct usage: JOIN <TableName> AND <TableName> " +
                    "ON <AttributeName> AND <AttributeName>");
            return;
        }
        if(!thirdWord().equalsIgnoreCase("AND")){
           session.send("Expected 'AND' following table name in JOIN statement");
           return;
        }
        if(!fifthWord().equalsIgnoreCase("ON")){
           session.send("Expected 'ON' following table name in JOIN statement");
           return;
        }
        if(!seventhWord().equalsIgnoreCase("AND")){
           session.send("Expected 'AND' following attribute name in JOIN statement");
           return;
        }
        Database currentDb = session.getCurrentDatabase();
        if (currentDb == null) return;
        Table table1 = currentDb.getTable(secondWord());
        Table table2 = currentDb.getTable(fourthWord());
        if(table1 != null && table2 != null){
            // The attributes are the 6th and 8th words
            String attribute1 = sixthWord();
            String attribute2 = eighthWord();
            Table joinedTable = currentDb.joinTables(table1, attribute1, table2, attribute2);
            if(joinedTable == null){
                session.send("Could not find the attribute in the table. Check spelling and case.");
            } else{
                session.send("JOINED TABLE: ");
                joinedTable.printTable(joinedTable.getTableArray());
            }
        }
    }



    // Parses the attribute list within the brackets and tokenises them to individual words.
    boolean tryTokeniseList(ArrayList<String> attributes){
        // Get the first and last words
        String first = attributes.get(0);
        String last = attributes.get(attributes.size()-1);
        // Check that the list begins with an open bracket and finishes with a closed bracket.
        if(first.charAt(0) != '(' || last.charAt(last.length()-1) != ')'){
            session.send("Expected brackets around attribute list.");
            return false;
        }
        // Remove opening bracket from first attribute.
        attributes.set(0, first.substring(1));
        // Remove closing bracket from last attribute.
        attributes.set(attributes.size()-1, last.substring(0, last.length()-1));
        return tryRemoveCommas(attributes);
    }


    // Removes commas from an attribute list
    private boolean tryRemoveCommas(ArrayList<String> attributes){
        // loop through removing all the commas (not including the last word).
        for(int i = 0; i < attributes.size()-1; i++){
            String currentWord = attributes.get(i);
            // Check the last letter in the word is a comma.
            if(currentWord.charAt(currentWord.length()-1) != ','){
                session.send("Expected comma to separate the attributes");
                return false;
            }
            // Remove the comma from the end of the word.
            String withoutCommas = attributes.get(i).substring(0, attributes.get(i).length() - 1);
            attributes.set(i, withoutCommas);
        }
        // if it reaches here, then all commas have been removed successfully and we can return true.
        return true;
    }




    public String firstWord(){
        if(input.size() < 1){
            return null;
        } else{
            return input.get(0);
        }
    }
    public String secondWord(){
        if(input.size() < 2){
            return null;
        } else{
            return input.get(1);
        }
    }
    public String thirdWord(){
        if(input.size() < 3){
            return null;
        } else{
            return input.get(2);
        }
    }
    public String fourthWord(){
        if(input.size() < 4){
            return null;
        } else{
            return input.get(3);
        }
    }
    public String fifthWord(){
        if(input.size() < 5){
            return null;
        } else{
            return input.get(4);
        }
    }
    public String sixthWord(){
        if(input.size() < 6){
            return null;
        } else{
            return input.get(5);
        }
    }
    public String seventhWord(){
        if(input.size() < 7){
            return null;
        } else{
            return input.get(6);
        }
    }
    public String eighthWord(){
        if(input.size() < 8){
            return null;
        } else{
            return input.get(7);
        }
    }






}
