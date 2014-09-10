
public class Vertex<T extends Comparable<T>> implements Comparable<Vertex<T>> {
	private T id;
	private int level;

	public Vertex(T id) {
		this.id = id;
		this.level = 0;
	}

	public T getId() {
		return this.id;
	}

	public int getLevel() {
		return this.level;
	}

	public void setLevel(int level) {
		this.level = level;
	}

	@Override
    public String toString() {
        return "Id: " + this.id.toString();
    }

	@Override 
    public boolean equals(Object obj) {
        if (!(obj instanceof Vertex))
            return false;
        if (obj == this)
            return true;
        return this.id.equals(((Vertex)obj).id);
    }
    
    @Override
    public int hashCode() {
        return 0;
    }

    @Override 
    public int compareTo(Vertex<T> v) {
    	return this.id.compareTo(v.id);
    }
}