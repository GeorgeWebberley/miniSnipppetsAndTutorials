import java.util.ArrayList;
import java.util.List;

public class Condition {
    private String attribute;
    private String operator;
    private String value;
    private Session session;


    Condition(String attribute, String operator, String value, Session session){
        this.attribute = attribute;
        this.operator = operator;
        this.value = value;
        this.session = session;
    }

    // Loops through the table and returns the index of every row which meets the requirements in the 'WHERE' statement
    List<Integer> findMatchingRows(ArrayList<ArrayList<String>> table){
        // Get the type of value (String, boolean, integer or float)
        String valueType = getTypeOfValue(this.value);
        if(valueType == null) return null;
        List<Integer> rowIndex = new ArrayList<>();
        // Get the index of the attribute from the first row of the table.
        int index = table.get(0).indexOf(this.attribute);
        // if the index of the attribute was not found then index equals -1
        if(index < 0) {
            session.send("Could not find attribute " + this.attribute + " in table");
            return null;
        }
        // Loop through each row (except from the first which contains the attribute names)
        for(int i = 1; i < table.size(); i++){
            // Get the value from the table and send it to a function which compares it with the value in the condition.
            if(compareWithTableValue(valueType, table.get(i).get(index))){
                // Add the index of the row to the array
                rowIndex.add(i);
            }
        }
        // Return the array of all the indices where the value matches the condition.
        return rowIndex;
    }


    String getTypeOfValue(String value){
        if(value.charAt(0) == '\'' && value.charAt(value.length()-1) == '\''){
            return "String";
        }
        if(value.equals("true") || value.equals("false")){
            return "Boolean";
        }
        try {
            Float.parseFloat(value);
            return "Number";
        } catch (NumberFormatException nfe){
            session.send("Did not recognise type of value " + value);
            session.send("Did you forget to use inverted commas around a string?");
            return null;
        }
    }

    private boolean compareWithTableValue(String valueType, String tableValue){
        switch(valueType){
            case "String":
                return compareStrings(tableValue);
            case "Boolean":
                return compareBooleans(tableValue);
            case "Number":
                try{
                    Float rowValue = Float.parseFloat(tableValue);
                    Float conditionValue = Float.parseFloat(value);
                    if(compareNumbers(rowValue, conditionValue)) return true;
                } catch (NumberFormatException nfe) {
                    session.send("Not able to compare " + tableValue + " and " + value);
                }
                break;
            default:
                session.send("Could not find attribute " + attribute);
                session.send("Check your conditional statement.");
        }
        return false;
    }


    boolean compareStrings(String rowValue){
        switch(operator.toUpperCase()){
            case "==":
                return rowValue.equals(value);
            case "!=":
                return !rowValue.equals(value);
            case "LIKE":
                return (rowValue.contains(value.replaceAll("^'|'$", "")));
            default:
                session.send("Cannot use '" + operator + "' on strings.");
        }
        return false;
    }

    boolean compareBooleans(String rowValue){
        switch(operator.toUpperCase()){
            case "==":
                return rowValue.equals(value);
            case "!=":
                return !rowValue.equals(value);
            default:
                session.send("Cannot use '" + operator + "' on strings.");
        }
        return false;
    }


    boolean compareNumbers(Float rowValue, Float conditionValue){
        switch(operator.toUpperCase()){
            case "==":
                return rowValue.equals(conditionValue);
            case "!=":
                return !rowValue.equals(conditionValue);
            case ">":
                return rowValue > conditionValue;
            case "<":
                return rowValue < conditionValue;
            case ">=":
                return rowValue >= conditionValue;
            case "<=":
                return rowValue <= conditionValue;
            default:
                session.send("Cannot use '" + operator + "' on integers and floats.");
        }
        return false;
    }

}
