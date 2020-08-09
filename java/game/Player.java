import java.util.ArrayList;

public class Player extends Character {
    private String location;
    private ArrayList<Artifact> inventory = new ArrayList<>();
    private int health;

    //----------Constructor----------
    Player(String name, String description, Location start) {
        super(name, description);
        this.location = start.getName();
        start.addEntity(this);
        health = 3;
    }

    // Retrieve a Player object from the name attribute.
    public static Player getPlayerFromName(String playerName, ArrayList<Player> playerList){
        for(Player p: playerList){
            if(p.getName().equals(playerName)){
                return p;
            }
        }
        return null;
    }

    //----------location management methods----------
    public void setLocation(String location){
        this.location = location;
    }

    public String getLocation(){
        return this.location;
    }

    public String changeLocation (String command, Location currentLocation, ArrayList<Location> locationList){
        for(String path: currentLocation.getPaths()){
            if(command.toLowerCase().contains(path)){
                return attemptMove(path, locationList, currentLocation);
            }
        }
        return "\nNot able to " + command + ". Type 'look' to check available paths.";
    }

    private String attemptMove (String path, ArrayList<Location> locationList, Location oldLocation){
        Location newLocation = Location.getLocationFromName(path, locationList);
        if(newLocation == null){
            return "Oops! The location " + path + " does not seem to exist!";
        }
        this.location = path;
        newLocation.addEntity(this);
        oldLocation.removeEntity(this);
        String output = "\nYou have now moved to the " + path + ".\n";
        output += newLocation.getEntitiesAsString(this);
        output += newLocation.getPathsAsString();
        return output;
    }



    //--------inventory management methods----------
    public ArrayList<Artifact> getInventory(){
        return this.inventory;
    }

    public String getInventoryAsString(){
        String inventoryString = "\nCurrent health: " + this.health + "\n";
        if(this.inventory.size() != 0){
            inventoryString += "\nYour inventory: \n";
            for(int i = 0; i < this.inventory.size(); i++){
                // add 1 to i when printing so the inventory list starts at 1.
                inventoryString += i+1 + ": " + this.inventory.get(i).getName() + "\n";
            }
        }else{
            inventoryString += "You do not have any items!";
        }
        return inventoryString;
    }

    public void addToInventory(Artifact artifact){
        this.inventory.add(artifact);
    }

    public void removeFromInventory(Artifact artifact){
        this.inventory.remove(artifact);
    }

    public String dropArtifact(String command, Location currentLocation){
        for(Artifact a : this.inventory){
            if(command.toLowerCase().contains(a.getName())){
                this.inventory.remove(a);
                currentLocation.addEntity(a);
                return "\nYou have dropped the " + a.getName();
            }
        }
        return "\nNot able to " + command + ". Check your inventory by typing 'inventory' or 'inv'.";
    }

    public String pickupArtifact(String command, Location currentLocation){
        for(Artifact a : currentLocation.getArtifacts()){
            if(command.toLowerCase().contains(a.getName())){
                currentLocation.removeEntity(a);
                this.inventory.add(a);
                return "\n" + a.getName() + " added to your inventory.";
            }
        }
        return "\nNot able to " + command + ". Try using 'look' to see artifacts and furniture in the room.";
    }

    public void clearInventory(){
        this.inventory.clear();
    }

    //----------health management methods----------
    public int getHealth(){
        return this.health;
    }

    public void setHealth(int x){
        this.health = x;
    }

    public void increaseHealth(int x){
        this.health += x;
    }

    public void decreaseHealth(int x){
        this.health -= x;
    }

}
