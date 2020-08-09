import java.util.ArrayList;

public class Entity {

    private String name;
    private String description;
    private String type;

    Entity(String name, String description, String type){
        this.name = name;
        this.description = description;
        this.type = type;
    }

    public String getDescription() {
        return description;
    }

    public String getName() {
        return name;
    }

    public String getType() {
        return type;
    }


    public static Entity getEntityFromName(String entityName, ArrayList<Location> locationList, ArrayList<Artifact> inventory){
        // First loop through the players inventory for possible artifacts.
        for(Artifact i : inventory){
            if(entityName.equals(i.getName())){
                return i;
            }
        }
        // Now loop through the location list and check if the name matches the location.
        for(Location l : locationList){
            if(entityName.equals(l.getName())){
                return l;
            }
            // Check Entities in each room.
            for(Entity e : l.getEntities()){
                if(entityName.equals(e.getName())){
                    return e;
                }
            }
        }
        // If all the above loops fail then the entity either does not exist or it is 'health' and we will return the original string.
        return null;
    }


}
