import java.util.ArrayList;
import java.util.Objects;

public class Commands {

    // Run the command through the command handlers for standard and special commands.
    String runCommand(String command, Player player, ArrayList<Location> locationList, ArrayList<Action> actionList){
        // Check the 'command' string with the special-command methods first (as special commands have stricter criteria)
        String output = runSpecialCommand(command, player, locationList, actionList);
        if(output != null){
            return output;
        }else {
            // If command is not a special command, to try running it as standard command.
            output = runStandardCommand(command, player, locationList);
            // If output is null (i.e. not standard or special command) to return an error string.
            return (Objects.requireNonNullElse(output, "\n-Please try a different command-"));
        }
    }

    // Checks the command with the standard commands and returns an output string (or null if it fails).
    private String runStandardCommand(String command, Player player, ArrayList<Location> locationList){
        Location currentLocation = Location.getLocationFromName(player.getLocation(), locationList);
        // error check to make sure the current location exists.
        if(currentLocation == null){
            return null;
        }
        // 'Inventory' command
        if(command.toLowerCase().contains("inventory") || command.toLowerCase().contains("inv")){
            return player.getInventoryAsString();
        }
        // 'Get' command
        if(command.toLowerCase().contains("get")){
            return player.pickupArtifact(command, currentLocation);
        }
        // 'Drop' command
        if(command.toLowerCase().contains("drop")){
            return player.dropArtifact(command, currentLocation);
        }
        // 'Goto' command
        if(command.toLowerCase().contains("goto")){
            return player.changeLocation(command, currentLocation, locationList);
        }
        // 'Look' command
        if(command.toLowerCase().contains("look")){
            String output = "\nYou are in the " + currentLocation.getName()
                    + " (" + currentLocation.getDescription() + ").\n";
            output += currentLocation.getEntitiesAsString(player);
            output += currentLocation.getPathsAsString();
            return output;
        }
        // If none of the above commands are recognised then the method returns null (so the game knows to check the special commands).
        return null;
    }

    // If triggers provided it will attempt to run that particular special-command and return the narration as a string.
    private String runSpecialCommand(String command, Player player, ArrayList<Location> locationList, ArrayList<Action> actionList){
        Location location = Location.getLocationFromName(player.getLocation(), locationList);
        // Loop through each possible action from the JSON file.
        for(Action action : actionList) {
            // Check to see if the command contains any of the 'trigger' key words for that action.
            if (triggersProvided(action, command)){
                if(action.attemptAction(location, player, locationList)){
                    return action.getNarration();
                }
            }
        }
        return null;
    }

    // Loops through all triggers for an action, returns true if the triggers were provided in the command.
    private boolean triggersProvided(Action action, String command){
        for(String trigger : action.getTriggers()){
            if(command.contains(trigger)){
                return true;
            }
        }
        return false;
    }

}
