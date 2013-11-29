import java.util.*;

class MyMultisetIterator<E> implements Iterator<E>{
	
	private MyMultiset multiset;
	private Iterator<E> innerIter;
	private E lastAnswerOfInnerIterator;
	boolean broken;

	private E currentItem;
	private int currentOccurance;

	MyMultisetIterator(MyMultiset<E> owner){
		lastAnswerOfInnerIterator = null;
		multiset = owner;
		Set<E> keySet = multiset.keySet();
		innerIter = keySet.iterator();
		if(innerIter.hasNext()){
			lastAnswerOfInnerIterator = innerIter.next();
		}
		currentOccurance = 0;
		broken = false;
	}

	private boolean lastAnswerOfInnerIteratorPointsToSMTHMeaningfull(){
		return multiset.contains(lastAnswerOfInnerIterator);
	}

	public boolean hasNext(){
		if(broken){
			return false;
		}
		if(lastAnswerOfInnerIteratorPointsToSMTHMeaningfull()){
		
			
			if( (innerIter.hasNext() == false) && currentOccurance >= multiset.count(lastAnswerOfInnerIterator) ){
				// then false

				return false;
			} else {
				// elsewhere - true
				return true;
			}
		} else {
			// here - we are broken;
			broken = true;
			return false;
		}
	}

	public E next(){
		if(broken){
			return null;
		}
		if(!hasNext()){
			return null;
		}
		// System.out.println("");
		// System.out.println("next");
		if(lastAnswerOfInnerIteratorPointsToSMTHMeaningfull()){


			if(currentOccurance++ >= multiset.count(lastAnswerOfInnerIterator)){
				lastAnswerOfInnerIterator = innerIter.next();
				currentOccurance = 0;
			}
			
			

			return lastAnswerOfInnerIterator;

		} else {
			// here - we are broken;
			broken = true;
			return null;
		}
	}

	
	public void remove(){
		if(broken){
			return;
		}
		
		if(multiset.count(lastAnswerOfInnerIterator) > 1){
			multiset.remove(lastAnswerOfInnerIterator);
			currentOccurance = Math.max(currentOccurance - 1, 0);
		} else {
			E lastAnswerOfInnerIteratorStored = lastAnswerOfInnerIterator;
			next();
			multiset.remove(lastAnswerOfInnerIteratorStored);
		}
		
	}
}