import com.alexmerz.graphviz.ParseException;
import com.alexmerz.graphviz.Parser;
import com.alexmerz.graphviz.objects.Edge;
import com.alexmerz.graphviz.objects.Graph;
import com.alexmerz.graphviz.objects.Node;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;

public class DotParser {

    void processFile(String file, ArrayList<Location> locationList) {
        FileReader in;
        File f = new File( file );
        Parser p = new Parser();
        try {
            in = new FileReader(f);
            p.parse(in);
            ArrayList<Graph> graphList = p.getGraphs();
            ArrayList<Graph> subGraphList = graphList.get(0).getSubgraphs();
            for(Graph g : subGraphList){
                loopLocationGraph(locationList, g);
                ArrayList<Edge> edges = g.getEdges();
                loopThroughEdges(edges, locationList);
            }
        } catch (FileNotFoundException | ParseException e) {
            System.out.println(e);
        }
    }

    private void loopLocationGraph(ArrayList<Location> locationList, Graph graph){
        ArrayList<Graph> locationGraphList = graph.getSubgraphs();
        for (Graph locationGraph : locationGraphList){
            ArrayList<Node> nodesLoc = locationGraph.getNodes(false);
            Node nLoc = nodesLoc.get(0);
            Location current = new Location(nLoc.getId().getId(), nLoc.getAttribute("description"));
            ArrayList<Graph> subGraphs2 = locationGraph.getSubgraphs();
            for (Graph locationSubGraph : subGraphs2) {
                loopEntityNodes(current, locationSubGraph);
            }
            locationList.add(current);
        }
    }

    private void loopEntityNodes(Location location, Graph locationSubGraph){
        ArrayList<Node> entityNodeList = locationSubGraph.getNodes(false);
        for (Node entityNode : entityNodeList) {
            String entityType = locationSubGraph.getId().getId();
            String name = entityNode.getId().getId();
            String description = entityNode.getAttribute("description");
            switch(entityType){
                case "artefacts":
                    location.addEntity(new Artifact(name, description));
                    break;
                case "furniture":
                    location.addEntity(new Furniture(name, description));
                    break;
                case "characters":
                    location.addEntity(new Character(name, description));
            }
        }
    }

    private void loopThroughEdges(ArrayList<Edge> edges, ArrayList<Location> locationList){
        for (Edge e : edges){
            for(Location l : locationList){
                if(l.getName().equals(e.getSource().getNode().getId().getId())){
                    l.addPath(e.getTarget().getNode().getId().getId());
                }
            }
        }
    }


}
