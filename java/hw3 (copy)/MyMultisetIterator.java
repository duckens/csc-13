import java.util.*;

class MyMultisetIterator<E> implements java.util.Iterator{
	
	private MyMultiset multiset;
	private Iterator<E> innerIter;
	private E lastAnswerOfInnerIterator;
	boolean broken;

	private E currentItem;
	private int currentOccurance;

	MyMultisetIterator(MyMultiset<E> owner){
		lastAnswerOfInnerIterator = null;
		innerIter = multiset.keySet().iterator();
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
			// if inner iterator points to the last key and there are no more occurances
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
		multiset.remove(lastAnswerOfInnerIterator);
		currentOccurance = Math.max(currentOccurance - 1, 0);
	}
}