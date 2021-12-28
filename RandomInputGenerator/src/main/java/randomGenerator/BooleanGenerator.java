package randomGenerator;

import java.util.Random;

public class BooleanGenerator {
	
	public String boolGenerator()
	{
		Boolean b = new Random().nextBoolean(); 
		return b.toString(); 
	}

}
