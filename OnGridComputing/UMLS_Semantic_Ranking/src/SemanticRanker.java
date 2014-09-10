
import java.io.*;
import java.util.*;

public class SemanticRanker {
	Graph<Vertex<String>, Edge<Vertex<String>>> graph;

	public SemanticRanker() {
		graph = new Graph<Vertex<String>, Edge<Vertex<String>>>();
	}

	public void run(String infile, String outfile, String dotfile, String degreefile) throws IOException {
		readFile(infile);
		writeDotGraph(dotfile);
		reverseGraphDirection();    // <--- reverse edges for ranking purposes
		Map<Vertex<String>, Integer> degree = graph.getAllDegreeOfIncoming();
		writeDegreeList(degree, degreefile);
		List<Vertex<String>> rankList = getRankList(degree);
		writeRankList(rankList, outfile);
	}

	private void writeDotGraph(String dotfile) throws IOException {
		BufferedWriter writer = new BufferedWriter(new FileWriter(dotfile));
		writer.write("digraph G {\nsize=\"25\";\nnode [color=lightblue2, style=filled];\n");

		for (Vertex<String> source : graph.getAdjacencyMap().keySet()) {
            for (Vertex<String> target : graph.getAdjacencyMap().get(source)) {
                writer.write("\"" + source.getId() + "\" -> \"" + target.getId() + "\";\n");
            }
        }

		writer.write("}");
		writer.close();
	}

	private void writeDegreeList(Map<Vertex<String>, Integer> degree, String degreefile) throws IOException {
		BufferedWriter writer = new BufferedWriter(new FileWriter(degreefile));
		for (Vertex<String> v : degree.keySet()) {
			writer.write(v.getId() + "\t" + degree.get(v) + "\n");
		}
		writer.close();
	}

	private void writeRankList(List<Vertex<String>> rankList, String outfile) throws IOException {
		Collections.sort(rankList);
		BufferedWriter writer = new BufferedWriter(new FileWriter(outfile));
		for (Vertex<String> v : rankList) {
			writer.write(v.getId() + "\t" + v.getLevel() + "\n");
		}
		writer.close();
	}

	private void readFile(String infile) throws IOException {
		BufferedReader reader = new BufferedReader(new FileReader(infile));
		String line;
		while ((line = reader.readLine()) != null) {
			String[] tokens = line.split("\\|");
			if (tokens[1].equals("isa")) {
				Vertex<String> source = new Vertex<String>(tokens[2]);
				Vertex<String> target = new Vertex<String>(tokens[0]);
				graph.addEdge(new Edge<Vertex<String>>(source, target), source, target);
			}
		}
		reader.close();
	}

	public void reverseGraphDirection() {
		LinkedHashSet<Edge<Vertex<String>>> reverseEdges = new LinkedHashSet<Edge<Vertex<String>>>();
		LinkedHashSet<Edge<Vertex<String>>> edges = graph.getVertexEdges();
		Iterator<Edge<Vertex<String>>> it = edges.iterator();
		while (it.hasNext()) {
			Edge<Vertex<String>> e = it.next();
			reverseEdges.add(new Edge<Vertex<String>>(e.getTarget(), e.getSource()));
		}
		graph.clearEdges();
		it = reverseEdges.iterator();
		while (it.hasNext()) {
			Edge<Vertex<String>> e = it.next();
			graph.addEdge(e, e.getSource(), e.getTarget());
		}
	}

    public List<Vertex<String>> getRankList(Map<Vertex<String>, Integer> degree) {
    	Stack<Vertex<String>> zeroVerts = new Stack<Vertex<String>>();
    	for (Vertex<String> v : degree.keySet()) {
    		if (degree.get(v) == 0) {
    			v.setLevel(1);
    			zeroVerts.push(v);
    		}
    	}
    	List<Vertex<String>> result = new ArrayList<Vertex<String>>();			// return results as map, containing level info
    	while (!zeroVerts.isEmpty()) {
    		Vertex<String> v = zeroVerts.pop();						// choose zero in-degree vertex
    		result.add(v);											// next in top order
    		for (Vertex<String> adjacent : graph.getAdjacencyMap().get(v)) {	// remove vertex from its neighbors
    			adjacent.setLevel(v.getLevel()+1);
    			degree.put(adjacent, degree.get(adjacent) - 1);
    			if (degree.get(adjacent) == 0)						// keep track of new vertices with zero in-degree
    				zeroVerts.push(adjacent);
    		}
    	}
    	if (result.size() != graph.getAdjacencyMap().size())		// if result not size of entire graph, there was a cycle
    		return null;
    	return result;
    }

	public static void main(String[] args) throws IOException {
		String indatafile = args[0];
		String outrankfile = args[1];
		String outdotfile = args[2];
		String outdegreefile = args[3];
		
		SemanticRanker sr = new SemanticRanker();
		sr.run(indatafile, outrankfile, outdotfile, outdegreefile);
	}

}
