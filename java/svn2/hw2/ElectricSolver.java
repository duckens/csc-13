import ru.compscicenter.java2013.electro.*;


public class ElectricSolver implements ElectricProblemSolver {

	private int i;

	private boolean IsSpacer(char ch){
		if(ch == '.' || ch == ',' || ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'){
			return true;
		}
		return false;
	}

	private boolean IsElectricalLetter(char ch){
		if(ch == 'I' || ch == 'R' || ch == 'U'){
			return true;
		}
		return false;
	}

	private boolean IsEquality(char ch){
		if(ch == '='){
			return true;
		}
		return false;
	}

	private boolean IsDigitOrDot(char ch){
		if(ch == '.' || ch == ',' || Character.isDigit(ch)){
			return true;
		}
		return false;	
	}
	
	private String ReadWholeUnitFromHere(String problemText){
		String str = new String("");
		while(!IsSpacer(problemText.charAt(i)) && i < problemText.length()){
			str += problemText.charAt(i);
			i++;
		}
		return str;
	}

	private double ReadWholeNumberFromHere(String problemText){
		String str = new String("0");
		while(IsDigitOrDot(problemText.charAt(i)) && i < problemText.length()){
			str += problemText.charAt(i);
			i++;
		}
		
		return Double.parseDouble(str);
	}

	private double CoefficientForUnit(String unit){
		// килоом (кОм), мегаом (МОм), гигаом (ГОм), миллиом (мОм), микроом (мкОм), наноом (нОм)
		char firstChar = unit.charAt(0);
		if(firstChar == 'к'){
			return 1000.0;
		} else if(firstChar == 'М'){
			return 1000000.0;
		} else if(firstChar == 'г'){
			return 1000000000.0;
		} else if(firstChar == 'н'){
			return 0.000000001;
		} else if(firstChar == 'м'){
			if(unit.charAt(1) == 'к'){
				return 0.000001;
			} else {
				return 0.001;
			}
		}
		return 1.0;
	}

	public ElectricQuantity solve(String problemText) throws SolverException {
 

		i = 0;
		int foundValues = 0;
		char lastElectricalLetter = 'N';

		double value = -1.0;
		String unit;

		boolean waitingForValue = false;
		boolean waitingForUnit = false;

		boolean gotValue = false;
		boolean gotUnit = false;

		ElectricQuantity allWeKnow[] = new ElectricQuantity[3];

		for(int i = 0; i < 2; i++){
			allWeKnow[i] = new ElectricQuantity(null, -1.0);
		}

		while(i < problemText.length()){

			char thisChar = problemText.charAt(i);
			if(IsElectricalLetter(thisChar)){

				lastElectricalLetter = thisChar;
				gotValue = false;
				gotUnit = false;
				waitingForValue = false;
				waitingForUnit = false;

			} else if(IsEquality(thisChar)){

				if(lastElectricalLetter != 'N'){
					waitingForValue = true;
				}

			} else if(waitingForValue){

				if(IsDigitOrDot(thisChar)){
				
					value = ReadWholeNumberFromHere(problemText);
					// System.out.println(value);
					gotValue = true;
					while(IsDigitOrDot(problemText.charAt(i)) && i < problemText.length()){
						//i++;
					}
					i--;
					waitingForValue = false;
					waitingForUnit = true;
				
				}

			} else if(waitingForUnit){

				if(IsSpacer(thisChar)){
					// pass
				} else {
					unit = ReadWholeUnitFromHere(problemText);
					i--;
					// System.out.println(unit);
					gotUnit = true;
					waitingForUnit = false;

					allWeKnow[foundValues] = new ElectricQuantity( ElectricQuantityType.forSymbol(lastElectricalLetter), value * CoefficientForUnit(unit) );

					// System.out.println( value * CoefficientForUnit(unit) );
					
					foundValues++;
					if(foundValues >= 2){
						break;
					}
				}

			}

			i++;
		}

		if(foundValues < 2){
			throw new SolverException("too few data");
		} else {
			// I = U / R => 1 = I * R / U;
			
			char[] symbols = new char[3];
			symbols[0] = 'I';
			symbols[1] = 'R';
			symbols[2] = 'U';
			boolean[] symbolsPresent = new boolean[3];
			symbolsPresent[0] = false;
			symbolsPresent[1] = false;
			symbolsPresent[2] = false;

			double res = 1.0;

			

			for(int i = 0; i < 2; i++){
				if(allWeKnow[i].getType() == null){
					throw new SolverException("unknown units");
				}
			}
			if(allWeKnow[0].getType() == allWeKnow[1].getType()){
				throw new SolverException("repeating data");
			}


			for(int i = 0; i < 2; i++){
				if(allWeKnow[i].getType() == ElectricQuantityType.VOLTAGE){
					res /= allWeKnow[i].getValue();
				} else {
					res *= allWeKnow[i].getValue();
				}
			}
			
			for(int i = 0; i < 2; i++){
				for(int j = 0; j < 2; j++){
					if(allWeKnow[i].getType() == ElectricQuantityType.forSymbol(symbols[j])){
						symbolsPresent[j] = true;
					}
				}
			}
			
			char res_type = 'I';
			for(int i = 0; i < 2; i++){
				if(symbolsPresent[i] == false){
					res_type = symbols[i];
				}
			}
			

			return new ElectricQuantity(ElectricQuantityType.forSymbol(res_type), res);
		}
	}


		
	/*
	public static void main(String[] args){
		String task = new String("Найдите сопротивление R участка цепи, если известно, что напряжение U = 10 В и сила тока I = 1 мА.");
		ElectricSolver es = new ElectricSolver();
		try {
			ElectricQuantity res = es.solve(task);
		} catch (SolverException ex) {
		
		}
	}
	*/
}