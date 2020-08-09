import java.util.ArrayList;

public class Action {
    private ArrayList<String> triggers = new ArrayList<>();
    private ArrayList<String> subjects = new ArrayList<>();
    private ArrayList<String> consumed = new ArrayList<>();
    private ArrayList<String> produced = new ArrayList<>();
    private String narration;


    // setters and getters
    public void setTriggers(ArrayList<String> triggers){
        this.triggers.addAll(triggers);
    }
    public void setSubjects(ArrayList<String> subjects){
        this.subjects.addAll(subjects);
    }
    public void setConsumed(ArrayList<String> consumed){
        this.consumed.addAll(consumed);
    }
    public void setProduced(ArrayList<String> produced){
        this.produced.addAll(produced);
    }
    public void setNarration(String narration){
        this.narration = narration;
    }

    public ArrayList<String> getTriggers(){
        return this.triggers;
    }
    public String getNarration(){
        return this.narration;
    }

    // --------  Methods below related to running a special action once triggered --------

    // If action succeeds, returns true. False otherwise.
    boolean attemptAction(Location location, Player player, ArrayList<Location> locationList){
        if(checkAllSubjectsPresent(location, player.getInventory())){
            removeAllConsumedSubjects(location, player, locationList);
            // returns true if able to generate 'produce' subjects, false otherwise.
            return tryProducingAllSubjects(location, player, locationList);
        }
        return false;
    }

    // Loops through array of subjects, checking the location and inventory to see if they are present.
    private boolean checkAllSubjectsPresent(Location location, ArrayList<Artifact> inventory){
        boolean subjectPresent;
        for(String subject : this.subjects){
            // Cast inventory to ArrayList<Entity> to send to 'checkOneSubject'
            subjectPresent = (checkOneSubject(subject, location.getEntities()) ||
                              checkOneSubject(subject, (ArrayList<Entity>)(ArrayList<?>)inventory));
            if(!subjectPresent){
                return false;
            }
        }
        // If it reaches the end of the for loop and hasn't returned false (for missing subject) then return true.
        return true;
    }

    // Checks individual subject is present in the ArrayList
    private boolean checkOneSubject(String subject, ArrayList<Entity> entityList){
        for(Entity e : entityList){
            if(e.getName().equals(subject)){
                return true;
            }
        }
        return false;
    }

    // loops through all subjects, calling 'removeOneConsumedSubject'.
    private void removeAllConsumedSubjects(Location location, Player player, ArrayList<Location> locationList){
        // Now we need to loop through all of the 'consumed' words and deal with them appropriately.
        for(String consumed : this.consumed){
            Entity subject = Entity.getEntityFromName(consumed, locationList, player.getInventory());
            removeOneConsumedSubject(location, player, subject);
        }
    }

    // Removes the individual subject (different result depending if the subject is health, entity in location/inventory or a path.)
    private void removeOneConsumedSubject(Location location, Player player, Entity subject){
        // the only 'non-entity' subject is health.
        if(subject == null){
            player.decreaseHealth(1);
            return;
        }
        switch (subject.getType()) {
            case "Furniture":
            case "Character":
                location.removeEntity(subject);
                break;
            case "Artifact":
                // Entities are unique. Therefore we can try removing artifact from both inventory and location safely.
                player.removeFromInventory((Artifact)subject);
                location.removeEntity(subject);
                break;
            case "Location":
                location.removePath(subject.getName());
        }
    }


    // returns true if able to remove all subjects, otherwise it returns false (i.e. if entity no longer exists in unplaced).
    private boolean tryProducingAllSubjects(Location location, Player player, ArrayList<Location> locationList) {
        Location unplaced = Location.getLocationFromName("unplaced", locationList);
        // First check if 'unplaced' location exists.
        if(unplaced == null){
            return false;
        }
//        Now we need to loop through all of the 'produced' words and deal with them appropriately.
        for (String produced : this.produced) {
            Entity subject = Entity.getEntityFromName(produced, locationList, player.getInventory());
            // If it fails to produce subject (i.e. subject no longer in unplaced) then return false.
            if (!tryProducingOneSubject(subject, player, location, unplaced)) {
                return false;
            }
        }
        return true;
    }


    // Returns true if successfully produces subject, otherwise returns false.
    private boolean tryProducingOneSubject(Entity subject, Player player, Location current, Location unplaced){
        // the only 'non-entity' subject is health.
        if(subject == null){
            player.increaseHealth(1);
            return true;
        }
        // need to get type of entity so we can deal with it appropriately.
        switch (subject.getType()) {
            case "Location":
                current.addPath(subject.getName());
                return true;
            case "Furniture":
            case "Character":
                // Returns false if not able to move entity from 'unplaced'
                return unplaced.tryMovingEntity(subject, current);
            case "Artifact":
                return unplaced.tryMovingArtifact(subject, player);
        }
        return false;
    }

}
