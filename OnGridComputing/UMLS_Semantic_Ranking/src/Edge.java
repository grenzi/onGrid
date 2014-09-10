

public class Edge<V> {
    private V source, target;
    
    public Edge(V source, V target) {
        this.source = source;
        this.target = target;
    }
    
    public V getSource() {
        return this.source;
    }

    public V getTarget() {
        return this.target;
    }
    
    @Override
    public String toString() {
        return "Source: " + source.toString() + ", Target: " + target.toString();
    }
    
    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Edge))
            return false;
        if (obj == this)
            return true;
        return (this.source.equals(((Edge)obj).source) && this.target.equals(((Edge)obj).target));
    }
    
    @Override
    public int hashCode() {
        return 0;
    }
    
}
