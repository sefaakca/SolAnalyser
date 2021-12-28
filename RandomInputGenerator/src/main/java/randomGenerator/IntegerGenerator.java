package randomGenerator;

import java.util.Random;
//import java.util.concurrent.ThreadLocalRandom;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class IntegerGenerator {
	
	public Random random = new Random();
	
	public double highNumber=1;
	public double signedlowNumber=0;
	public static final int UnsignedlowNumber = 1;
	public int randomNumber=0;
	public enum IntTypes{
		INT256,
		INT128,
		INT64,
		INT32,
		INT,
		UINT256,
		UINT128,
		UINT64,
		UINT32,
		UINT;
		
		
	}
	
	public String GenerateInteger(String type)
	{
		
		type=type.toUpperCase();
		
		switch(type) {
		case "INT256":
			highNumber = (Math.pow(2, 255)-1);
			signedlowNumber = (int) (Math.pow(-2, 255)-1);
			
			randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
			break;
		case "INT":
			highNumber = (Math.pow(2, 255)-1);
			signedlowNumber = (int) (Math.pow(-2, 255)-1);
			
			randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
			break;
		case "INT128":
			highNumber = (Math.pow(2, 127)-1);
			signedlowNumber = (int) (Math.pow(-2, 127)+1);
			randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber); 
			break;
		case "INT64":
			highNumber = (Math.pow(2, 63)-1);
			signedlowNumber = (int) (Math.pow(-2, 63)+1);
			randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
			break;
		case "INT32":
			highNumber = (Math.pow(2, 32)-1);
			signedlowNumber = (int) (Math.pow(-2, 32)+1);
			randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
			break;
		case "UINT":
		case "UINT256":
			highNumber = (Math.pow(2, 255)-1);
			randomNumber = random.nextInt((int)highNumber - UnsignedlowNumber + 1);
			break;
		case "UINT128":
			highNumber = (Math.pow(2, 127)-1);
			randomNumber = random.nextInt((int)highNumber - UnsignedlowNumber + 1);
			break;
		case "UINT64":
			highNumber = (Math.pow(2, 63)-1);
			randomNumber = random.nextInt((int)highNumber - UnsignedlowNumber + 1);
			break;
		case "UINT32":
			highNumber = (Math.pow(2, 31)-1);
			randomNumber = random.nextInt((int)highNumber - UnsignedlowNumber + 1);
			break;
				
		}
		return Integer.toString(randomNumber);
		
	}
	
	public int[] GenerateIntegerArray(String type) {
		
		int[] integerArr;

		String pattern ="\\[[^\\d]*(\\d+)[^\\d]*\\]";
		Pattern p = Pattern.compile(pattern, Pattern.MULTILINE);
		Matcher _mnum=p.matcher(type);
		int blen=0;
		if(_mnum.find()) {
			String match = _mnum.group();
			match =match.replace("[", "");
			match = match.replace("]", "");
			blen= Integer.parseInt(match);
			
			integerArr = new int[blen];
			type=type.substring(0,type.length()-(match.length()+2));
		}
		else{
			integerArr= new int[10];
			type=type.substring(0,type.length()-2);
		}
		
		type=type.toUpperCase();
		
		switch(type) {
		case "INT256":
			highNumber = (Math.pow(2, 255)-1);
			signedlowNumber = (int) (Math.pow(-2, 255)-1);
			for(int i=0;i<integerArr.length;i++) {
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				//integerArr[i] =  Integer.toString(randomNumber);
				integerArr[i] = randomNumber;
			}
				
			break;
		case "INT":
			highNumber = (Math.pow(2, 255)-1);
			signedlowNumber = (int) (Math.pow(-2, 255)-1);
			for(int i=0;i<integerArr.length;i++) {
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				integerArr[i]= randomNumber;
			}
				
			break;
		case "INT128":
			highNumber = (Math.pow(2, 127)-1);
			signedlowNumber = (int) (Math.pow(-2, 127)+1);
			randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber); 
			for(int i=0;i<integerArr.length;i++) {
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				integerArr[i]= randomNumber;
			}
			break;
		case "INT64":
			highNumber = (Math.pow(2, 63)-1);
			signedlowNumber = (int) (Math.pow(-2, 63)+1);
			randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
			for(int i=0;i<integerArr.length;i++) {
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				integerArr[i]= randomNumber;
			}
			break;
		case "INT32":
			highNumber = (Math.pow(2, 32)-1);
			signedlowNumber = (int) (Math.pow(-2, 32)+1);
			randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
			for(int i=0;i<integerArr.length;i++) {
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				integerArr[i]= randomNumber;
			}
			break;
		case "UINT":
		case "UINT256":
			highNumber = (Math.pow(2, 255)-1);
			randomNumber = random.nextInt((int)highNumber - UnsignedlowNumber + 1);
			for(int i=0;i<integerArr.length;i++) {
				
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				integerArr[i]= randomNumber;
			}
			break;
		case "UINT128":
			highNumber = (Math.pow(2, 127)-1);
			randomNumber = random.nextInt((int)highNumber - UnsignedlowNumber + 1);
			for(int i=0;i<integerArr.length;i++) {
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				integerArr[i]= randomNumber;
			}
			break;
		case "UINT64":
			highNumber = (Math.pow(2, 63)-1);
			randomNumber = random.nextInt((int)highNumber - UnsignedlowNumber + 1);
			for(int i=0;i<integerArr.length;i++) {
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				integerArr[i]= randomNumber;
			}
			break;
		case "UINT32":
			highNumber = (Math.pow(2, 31)-1);
			randomNumber = random.nextInt((int)highNumber - UnsignedlowNumber + 1);
			for(int i=0;i<integerArr.length;i++) {
				randomNumber = (int) (random.nextDouble()*((highNumber-signedlowNumber)+1)+signedlowNumber);
				integerArr[i]= randomNumber;
			}
			break;
				
		}
		return integerArr;
		
	}

}
