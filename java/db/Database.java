import java.util.ArrayList;

public class Database {
    private String name;
    private ArrayList<Table> tables = new ArrayList<>();
    private Session session;

    Database (String name, Session session){
        this.name = name;
        this.session = session;
    }

    public String getName() {
        return this.name;
    }

    public  ArrayList<Table> getTables(){
        return this.tables;
    }


    Table createTable(String tableName, ArrayList<String> attributes){
        Table newTable = new Table(tableName, attributes, session);
        tables.add(newTable);
        return newTable;
    }



    void dropTable(Table table){
        tables.remove(table);
    }


    Table getTable(String tableName){
        for(Table t : tables){
            if(t.getName().equals(tableName)){
                return t;
            }
        }
        // If it reaches the end of the for loop, then that table doesn't exist in this database.
        session.send("Couldn't find table with name '" + tableName + "' in database '" + this.name + "'.");
        return null;
    }

//    Joins 2 tables and returns the table object
    Table joinTables(Table tableObject1, String attribute1, Table tableObject2, String attribute2){
//      extract the tables (2d array list) from the table objects
        ArrayList<ArrayList<String>> table1 = tableObject1.getTableArray();
        ArrayList<ArrayList<String>> table2 = tableObject2.getTableArray();
//      get the attribute row of table1 and table2 and find the index of the attribute (if it exists)
        int table1Column = table1.get(0).indexOf(attribute1);
        int table2Column = table2.get(0).indexOf(attribute2);
        if(table1Column == -1 || table2Column == -1){
            return null;
        }
//      Create a new attribute list (a combination of the attributes from the 2 tables)
        ArrayList<String> newAttributes = new ArrayList<>();
//      Add the attributes from each table to the list
        addAttributes(newAttributes, tableObject1);
        addAttributes(newAttributes, tableObject2);
//      Create a table with the new attributes
        Table joinedTable = new Table("joined", newAttributes, session);
        for(int i = 1; i < table1.size(); i++){
            for(int j = 1; j < table2.size(); j++){
//              If the rows match on the specified column
                if(table1.get(i).get(table1Column).equals(table2.get(j).get(table2Column))){
                    ArrayList<String> newRow = new ArrayList<>();
                    addToNewRow(newRow, table1.get(i));
                    addToNewRow(newRow, table2.get(j));
                    joinedTable.insertValues(newRow);
                }
            }
        }
        return joinedTable;
    }

    // Adds attributes from the old table, appending the tablename to the start of the attribute
    void addAttributes(ArrayList<String> attributeList, Table table){
        // Add all the attributes from the previous table
        for(String attribute : table.getTableArray().get(0)){
            attributeList.add(table.getName() +"." +attribute);
        }
        // Remove the id attribute (as a new id will be made in the new table)
        attributeList.remove(table.getName() +"." +"id");
    }


    void addToNewRow(ArrayList<String> newRow, ArrayList<String> row){
        // the index of the id will always be 0, so we can start our loop at 1.
        for(int i = 1; i < row.size(); i++){
            newRow.add(row.get(i));
        }
    }


}
