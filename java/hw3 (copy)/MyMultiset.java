import java.util.*;


public class MyMultiset<E> implements ru.compscicenter.java2013.collections.MultiSet<E>{

	private final HashMap<E, Integer> content;

	MyMultiset(){
		content = new HashMap<E, Integer>();
	}

	MyMultiset(E[] array) {
		content = new HashMap<E, Integer>();
		for(E e: array) content.put(e, 1);
	}

	public boolean add(E e){
		Integer occurancesInside = content.get(e);
		if(occurancesInside == null){
			content.put(e, 1);
		} else {
			content.put(e, occurancesInside + 1);
		}
		return true;
	}

	public int add(E e, int occurrences){
		int occurancesInside = content.get(e);
		if(occurancesInside == 0){
			content.put(e, occurrences);
		} else {
			content.put(e, occurancesInside + occurrences);
		}
		return occurancesInside;
	}
	
	public void clear(){
		content.clear();
	}

	public boolean contains(Object o){
		return content.containsKey(o);
	}

	public boolean containsAll(Collection<?> c){
		boolean res = true;
		for(Object o: c) res &= contains(o);
		return res;
	}

	public int count(Object e){
		int res = content.containsKey(e) ? content.get(e) : 0;
		return res;
	}
		
	public int size(){
		int res = 0;
		for(E key: content.keySet()) res += content.get(key);
		return res;
	}

	public boolean equals(Object o){
		if(o instanceof MyMultiset){
			Iterator<E> iteratorInner = keySet().iterator();
			Iterator<E> iteratorOuter = ((MyMultiset)o).keySet().iterator();
			
			while(iteratorInner.hasNext() && iteratorOuter.hasNext()){
				E innerKey = iteratorInner.next();
				E outerKey = iteratorOuter.next();
				if( (!innerKey.equals(outerKey)) || count(innerKey) != count(outerKey) ) return false;
			}
			return ( (!iteratorInner.hasNext()) && (!iteratorOuter.hasNext()) );
		} else {
			return false;
		}
	}

	public Iterator<E> iterator(){
		return new MyMultisetIterator<E>(this);
	}
	
	public boolean remove(Object e){
		if(content.containsKey(e)){
			int occurancesInside = content.get(e);
			if(occurancesInside == 1){
				content.remove(e);
			} else {
				content.put((E)e, occurancesInside - 1);
			}
			return true;
		} else {
			return false;
		}
	}

	public boolean retain(Object e){
		if(content.containsKey(e)){
			content.remove(e);
			return true;
		} else {
			return false;
		}
	}

	public int remove(Object e, int occurrences){
		if(content.containsKey(e)){
			int occurancesThereWere = content.get(e);
			int occurancesShoulBeNow = occurancesThereWere - occurrences;

			if(occurancesShoulBeNow <= 0){
				occurancesShoulBeNow = 0;
				content.remove(e);
			} else {
				content.put((E)e, occurancesShoulBeNow);
			}
			return occurancesThereWere;
			
		} else {
			return 0;
		}
	}

	public boolean removeAll(Collection<?> c){
		boolean res = false;
		for(Object o: c) res |= remove(o);
		return res;
	}

	public boolean retainAll(Collection<?> c){
		boolean res = false;
		for(Object o: c) res |= retain(o);
		return res;
	}

	public Object[] toArray(){
		Object[] res = new Object[size()];
		Iterator<E> it = iterator();
		int pos = 0;
		while(it.hasNext()){
			res[pos++] = it.next();
		}
		return res;
	}

	public Set<E> keySet(){
		return content.keySet();
	}

	public <T> T[] toArray(T[] a){
		
		
		if(a == null || a.length < size()){
			return (T[])toArray();
		} else {
			Arrays.fill(a, null);
			Iterator<E> it = iterator();
			int pos = 0;
			while(it.hasNext()){
				a[pos++] = (T)it.next();
			}
			return a;
		}
	
	}

	public boolean addAll(Collection<? extends E> c){
		boolean res = true;
		for(Object o: c) res &= add((E)o);
		return res;

	}

	public boolean isEmpty(){
		return (size() == 0);

	}
}