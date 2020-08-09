import java.util.ArrayList;

public class Location extends Entity{

    // A list of all entities in the location (artifacts, characters and furniture).
    private ArrayList<Entity> entities = new ArrayList<>();
    // A list of artifacts only (has its own arraylist as the player can interact with it by picking up/dropping and moving it.
    private ArrayList<Artifact> artifacts = new ArrayList<>();
    // A list of locations (stored as name string) available for the player to travel to from this location.
    private ArrayList<String> paths = new ArrayList<>();


    Location(String name, String description) {
        super(name, description, "Location");
    }

    // getters
    public ArrayList<Entity> getEntities(){
        return this.entities;
    }

    public ArrayList<Artifact> getArtifacts(){
        return this.artifacts;
    }

    public ArrayList<String> getPaths(){
        return this.paths;
    }

    // Retrieve a Location object from the name attribute.
    public static Location getLocationFromName(String locationName, ArrayList<Location> locationList){
        for(Location l: locationList){
            if(l.getName().equals(locationName)){
                return l;
            }
        }
        System.err.println("Not able to find location with name: " + locationName);
        return null;
    }

    // Get ArrayLists as strings for printing.
    public String getEntitiesAsString(Player player){
        String entityString = "";
        // If there is 1 entity then it must be the player
        if(this.entities.size() > 1){
            entityString += "You can see:";
        }
        for(Entity e : this.entities){
            if(e != player){
                entityString += "\n\t- " + e.getName() + " (" + e.getDescription() + ").";
            }
        }
        return entityString;
    }

    // get all paths as a string for printing.
    public String getPathsAsString(){
        String pathsString = "\nYou can see paths leading to: ";
        for(String p : this.paths){
            pathsString += "\n\t- " + p;
        }
        return pathsString;
    }

    // path management methods
    public void addPath(String path){
        this.paths.add(path);
    }

    public void removePath(String path){
        this.paths.remove(path);
    }

    // entity management methods
    public void addEntity(Entity entity){
        this.entities.add(entity);
        // If the entity is an artifact then also need to add to the artifact arraylist
        if(entity.getType().equals("Artifact")){
            this.artifacts.add((Artifact) entity);
        }
    }

    public void removeEntity(Entity entity){
        this.entities.remove(entity);
        // If the entity is an artifact then also need to remove it from the artifact arraylist
        if(entity.getType().equals("Artifact")){
            this.artifacts.remove((Artifact) entity);
        }
    }


    // Moves an entity from one location to another. Returns true if successful, false otherwise
    public boolean tryMovingEntity(Entity entity, Location moveTo){
        for(Entity e : this.entities){
            if(e == entity){
                this.removeEntity(entity);
                moveTo.addEntity(entity);
                return true;
            }
        }
        return false;
    }

    // Moves entity from location to the player inventory (if it exists). Returns true if successful, false otherwise
    public boolean tryMovingArtifact(Entity entity, Player player){
        for(Entity a : this.artifacts){
            if(a == entity){
                // remove artifact from location and add to player inventory.
                this.removeEntity(entity);
                player.addToInventory((Artifact) entity);
                return true;
            }
        }
        return false;
    }

}
