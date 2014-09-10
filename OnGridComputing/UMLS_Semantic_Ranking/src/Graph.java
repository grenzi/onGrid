
import java.util.*;
import java.io.*;

public class Graph<V, E> {
	private Map<V, List<V>> adjacencyMap;
    private LinkedHashSet<E> vertexEdges;
    private ArrayList<V> vertexList;

	public Graph() {
		adjacencyMap = new TreeMap<V, List<V>>();
        vertexEdges = new LinkedHashSet<E>();
        vertexList = new ArrayList<V>();
	}

	public boolean addVertex(V v) {
        if (containsVertex(v))
            return false;
        else {
            adjacencyMap.put(v, new ArrayList<V>());
            vertexList.add(v);
        }
        return true;
    }

	public boolean addEdge(E e, V source, V target) {
        if (containsEdge(e))
            return false;
        else {
        	addVertex(source);		// in case source or target are not yet added
        	addVertex(target);
            adjacencyMap.get(source).add(target);
            vertexEdges.add(e);
        }
        return true;
    }

    public void clearEdges() {
        vertexEdges.clear();
        adjacencyMap.clear();
        vertexList.clear();
    }

    public boolean containsEdge(E e) {
        return vertexEdges.contains(e);
    }
    
    public boolean containsVertex(V v) {
        return adjacencyMap.containsKey(v);
    }

    public Map<V, List<V>> getAdjacencyMap() {
        return this.adjacencyMap;
    }

    public LinkedHashSet<E> getVertexEdges() {
        return this.vertexEdges;
    }

    public int getNumEdges() {
        return this.vertexEdges.size();
    }

    public int getNumVertices() {
        return this.adjacencyMap.size();
    }
    
    public ArrayList<V> getVertices() {
        return this.vertexList;
    }

    public int degreeOfIncoming(V v) {  // might be a more efficient way to compute this
        int count = 0;
        for (V x : adjacencyMap.keySet()) {
            if (!x.equals(v)) {
                List<V> list = adjacencyMap.get(x);
                for (V y : list) {
                    if (y.equals(v))
                        count++;
                }
            }
        }
        return count;
    }
    
    public int degreeOfOutgoing(V v) {
        List<V> list = adjacencyMap.get(v);
        if (list == null)
            return 0;
        else
            return list.size();
    }

    public Map<V, Integer> getAllDegreeOfIncoming() {
    	Map<V,Integer> result = new TreeMap<V,Integer>();
    	for (V v : adjacencyMap.keySet()) {
    		result.put(v, 0);
    	}
    	for (V source : adjacencyMap.keySet()) {
    		for (V target : adjacencyMap.get(source)) {
    			result.put(target, result.get(target) + 1);
    		}
    	}
    	return result;
    }
}