import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;


public class JsonParser {

    @SuppressWarnings("unchecked")
    void processFile(String file, ArrayList<Action> actionList){
        JSONParser parser = new JSONParser();

        try(FileReader reader = new FileReader(file)){
            JSONObject jsonObject = (JSONObject) parser.parse(reader);
            JSONArray actionsArray = (JSONArray) jsonObject.get("actions");
            for( int i = 0; i < actionsArray.size(); i++){
                Action playerAction = new Action();
                JSONObject jsonAction = (JSONObject) actionsArray.get(i);
                ArrayList<String> triggers = (ArrayList<String>) jsonAction.get("triggers");
                playerAction.setTriggers(triggers);
                ArrayList<String> subjects = (ArrayList<String>) jsonAction.get("subjects");
                playerAction.setSubjects(subjects);
                ArrayList<String> consumed = (ArrayList<String>) jsonAction.get("consumed");
                playerAction.setConsumed(consumed);
                ArrayList<String> produced = (ArrayList<String>) jsonAction.get("produced");
                playerAction.setProduced(produced);
                playerAction.setNarration((String)jsonAction.get("narration"));

                actionList.add(playerAction);
            }
        } catch (ParseException | IOException e) {
            e.printStackTrace();
        }
    }

}
