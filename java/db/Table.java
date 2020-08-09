import java.util.*;

public class Table {
//    Create a 2d arraylist to represent the table. The first entry in each sub-arraylist will be the attribute name.
    private ArrayList<ArrayList<String>> table = new ArrayList<>();
    private String name;
    private int highestRow = 0;
    private int numberOfAttributes = 0;
    private Session session;

    // Standard constructor, for creating a table from an sql query
    Table(String name, ArrayList<String> attributeList, Session session) {
        this.name = name;
        // Create one space for the 0th row
        this.table.add(new ArrayList<>());
        // Set the first attribute to be 'id' on the 0th row
        this.table.get(highestRow).add("id");
        // Loop through attribute list adding to the 0th row.
        // The 0th row will contain all of the attribute names. The following rows will be the data.
        for(String attribute : attributeList){
            this.table.get(highestRow).add(attribute);
            this.numberOfAttributes++;
        }
        highestRow = 1;
        this.session = session;
    }

    // Overloaded method. Create a table from a 2d ArrayList (when loaded from file as opposed to from a sql query).
    Table(String name, ArrayList<ArrayList<String>> cells, int numberOfAttributes, int highestRow, Session session){
        this.name = name;
        this.table = cells;
        // Number of attributes does not include the id so we can subtract 1
        this.numberOfAttributes = numberOfAttributes;
        this.highestRow = highestRow;
        this.session = session;
    }

    public String getName(){
        return this.name;
    }

    public ArrayList<ArrayList<String>> getTableArray(){
        return this.table;
    }



    void addOrDropAttribute(String alterationType, String attributeName){
        if(alterationType.toUpperCase().equals("ADD")){
            table.get(0).add(attributeName);
            populateColumnWithEmptyStrings();
            numberOfAttributes++;
        } else if (alterationType.toUpperCase().equals("DROP")){
           deleteAttributeAndColumn(attributeName);
        } else {
            session.send("Expected 'ADD' or 'DROP' following table name in 'ALTER' command.");
        }
    }

    // Populates the rest of the column with emptry strings (when a new column is added)
    void populateColumnWithEmptyStrings(){
        // Start at index 1 as we don't want to add an empty string at the end of the attribute row
        for(int i = 1; i < table.size(); i++){
            table.get(i).add("");
        }
    }

    // Deletes a column with given attribute name
    void deleteAttributeAndColumn(String attributeName){
        // Get the index of the attribute from the first row
        int index = table.get(0).indexOf(attributeName);
        // If the attribute exists in the table
        if(index >= 0){
            // loop through each row removing the cell relating to that attribute.
            for(ArrayList<String> row : table){
                row.remove(index);
            }
            numberOfAttributes--;
        } else{
            session.send(attributeName + " attribute not found in " + name);
            session.send(("Check the character cases and spelling"));
        }
    }

    // Inserts given values into the table at the highest row
    boolean insertValues(ArrayList<String> values){
        if(values.size() != numberOfAttributes) return false;
        table.add(new ArrayList<>());
        // Get the highest row, and insert the id (which is the same value as the highest row)
        table.get(highestRow).add(String.valueOf(highestRow));
        // Loop through all the values, inserting them into the highest row
        for(String value : values){
            table.get(highestRow).add(value);
        }
        // Increment the highest row, ready for the next row to be added.
        highestRow++;
        return true;
    }



    // Filter the table columns based on the selected attributes
    ArrayList<ArrayList<String>> filterTableColumns(List<Integer> indexList, ArrayList<ArrayList<String>> currentTable){
        ArrayList<ArrayList<String>> selectTable = new ArrayList<>();
        // loop through each row
        for(int i = 0; i < currentTable.size(); i++){
            selectTable.add(new ArrayList<>());
            // loop through each column
            for(Integer index : indexList){
                // Add the value from the old table to the new selectTable
                selectTable.get(i).add(currentTable.get(i).get(index));
            }
        }
        return selectTable;
    }

    // Prints table to user
    void printTable(ArrayList<ArrayList<String>> resultTable){
        for(ArrayList<String> row : resultTable){
            StringBuilder rowString = new StringBuilder();
            for(String cell : row){
                // justify the output by 23 (randomly chosen number to give enough space!)
                rowString.append(String.format("%-23s", cell));
            }
            session.send(rowString.toString());
            rowString.setLength(0);
        }
    }

    //Returns an index array of all the attributes specified in the select statement
    List<Integer> getColumnIndices(ArrayList<String> attributes){
        List<Integer> indexArray = new ArrayList<>();
        // loop through all the attributes in the select query
        for (String attribute : attributes) {
            // Get the index of that attribute in the first row of the table.
            Integer index = findOneAttribute(attribute);
            // if index is -1 then that attribute doesn't exist in the table
            if (index == -1) {
                session.send("Could not find attribute " + attribute + " in table.");
                return null;
            } else indexArray.add(index);
        }
        return indexArray;
    }

    // Finds an attribute index given the string
    Integer findOneAttribute(String attribute){
        // loop through all the attributes stored in the first row of the table
        for(int j = 0; j < table.get(0).size(); j++){
            // if the attribute in the array matches that in the table, return it.
            if(attribute.equals(table.get(0).get(j))) {
                return j;
            }
        }
        // if we reach the end of the for loop then that attribute doesn't exist in the table. Return -1 to show this.
        return -1;
    }



    // This function recursively works through the conditions, processing in the order specified by the braces
    List<Integer> getRowIndices(String conditionString){
        // either splits the condition into into separate conditions, or into individual words if it is just a single condition.
        ArrayList<String> conditionArray = tokeniseCondition(conditionString);
        if(conditionArray.size() != 3){
            session.send("Incorrect conditional statement. Please check and try again.");
            return null;
        }
        if(!conditionArray.get(1).toUpperCase().equals("AND") && !conditionArray.get(1).toUpperCase().equals("OR")){
            // This is our base case for the recursive function. If our condition doesn't contain AND/OR then we can
            // assume it is a single condition. The first word is the attribute, the second the operator and the third the value.
            Condition condition = new Condition(conditionArray.get(0), conditionArray.get(1), conditionArray.get(2), session);
            // Return a list of rows that match the condition
            return condition.findMatchingRows(this.table);
        }
        // Else, the condition is made up of multiple parts, with surrounding braces
        String condition1 = removeOuterBraces(conditionArray.get(0));
        String condition2 = removeOuterBraces(conditionArray.get(2));
        if(condition1 == null || condition2 == null) return null;
        if(conditionArray.get(1).toUpperCase().equals("AND")) {
            return intersect(getRowIndices(condition1), getRowIndices(condition2));
        }
//      If we have reached here the middle word must be 'OR' and can be treated with a union
        return union(getRowIndices(condition1), getRowIndices(condition2));
    }


    // This function parses the condition string. A condition can be made up of multiple conditions.
    // Therefore this function will either return 2 conditions (separated by AND/OR) or it will return a single condition, split into words
    ArrayList<String> tokeniseCondition(String conditionString){
        ArrayList<String> conditionArray = new ArrayList<>();
        StringBuilder token = new StringBuilder();
        // Intiialise some flags so we can check if we are inside quotation marks (a string) or inside braces (a condition)
        boolean insideString = false;
        int bracesCount = 0;
        for(int i = 0; i < conditionString.length(); i++){
            // As we enter a string, insideAString becomes true. As we leave, it becomes false.
            if (conditionString.charAt(i)=='\'') insideString = !insideString;
            // As we enter braces, we add 1 to the count. As we leave we subtract 1
            else if (conditionString.charAt(i)=='(') bracesCount++;
            else if (conditionString.charAt(i)==')') bracesCount--;
            // If we reach a space character, and we are not inside a string or within braces
            if (conditionString.charAt(i)==' ' && !insideString && bracesCount == 0) {
                // We add the token to the array
                conditionArray.add(token.toString());
                // Reset the string builder.
                token.setLength(0);
            }
            else token.append(conditionString.charAt(i));
        }
        conditionArray.add(token.toString());
        return conditionArray;
    }



    //  Removes the outer most braces from a condition
    String removeOuterBraces(String conditionString){
        if(conditionString.charAt(conditionString.length()-1) != ')'){
            session.send("Missing a closing brace in 'WHERE' condition.");
            return null;
        } else {
            return conditionString.substring(1, conditionString.length()-1);
        }
    }


    // If the conditions are separated by 'OR' then we can union the lists.
    private List<Integer> union(List<Integer> indexList1, List<Integer> indexList2){
        if(indexList1 == null || indexList2 == null) return null;
        // Create a set
        Set<Integer> set = new HashSet<>();
        // Add both index lists to the set
        set.addAll(indexList1);
        set.addAll(indexList2);
        // Return it as an arraylist.
        return new ArrayList<>(set);
    }

    // If the conditions are separated by 'AND' then we can intersect the lists.
    private List<Integer> intersect(List<Integer> indexList1, List<Integer> indexList2){
        List<Integer> intersection = new ArrayList<>();
        for(Integer index : indexList1){
            if(indexList2.contains(index)){
                intersection.add(index);
            }
        }
        return intersection;
    }


    // Filter the table rows based on the results of the condition (saved in the rowIndices).
    ArrayList<ArrayList<String>> filterTableRows(List<Integer> rowIndices){
        if(rowIndices == null || rowIndices.size() == 0) return null;
        ArrayList<ArrayList<String>> newTable = new ArrayList<>();
        // Add the attribute row onto the table so that when printed, the user can see the column headings
        newTable.add(this.table.get(0));
        for(int i = 1; i < this.table.size(); i++){
            if(rowIndices.contains(i)){
                newTable.add(this.table.get(i));
            }
        }
        return newTable;
    }





    void updateTable(List<Integer> rowIndices, HashMap<String, String> nameValuePairs){
        // loop through the first row (the attributes) in the table.
        for(Map.Entry<String, String> nameValue : nameValuePairs.entrySet()){
            for(int i = 0; i < table.get(0).size(); i++){
                if(table.get(0).get(i).equals(nameValue.getKey())){
                   updateValues(nameValue.getValue(), i, rowIndices);
                }
            }
        }
    }

    void updateValues(String newValue, int index, List<Integer> rowIndices){
//      Loop down the table, starting at the second row (index 1) to avoid the attribute row.
        for(Integer row : rowIndices){
//           Set the new value to each cell at that index
            table.get(row).set(index, newValue);
        }
    }

    void deleteRows(List<Integer> rowIndices){
        Iterator<ArrayList<String>> rowIterator = table.iterator();
        int i = 0;
        while(rowIterator.hasNext()){
            rowIterator.next();
            if(rowIndices.contains(i)){
                rowIterator.remove();
            }
            i++;
        }
    }


}
