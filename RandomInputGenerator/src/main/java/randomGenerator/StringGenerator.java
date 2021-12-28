package randomGenerator;

import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class StringGenerator {
	
	private int hex_down = 65; // letter 'A'
    private int hex_up = 122; // letter 'z'
    private int targetStringLength = 10;
    private Random random = new Random();
	public String stringGenerator()
	{
		
	    
	    StringBuilder buffer = new StringBuilder(targetStringLength);
	    for (int i = 0; i < targetStringLength; i++) {
	        int randomLimitedInt = hex_down + (int) 
	          (random.nextFloat() * (hex_up - hex_down + 1));
	        buffer.append((char) randomLimitedInt);
	    }
	    String generatedString = buffer.toString();
	    return generatedString;
	}
	
	public String[] stringArrGenerator(String type) 
	{
		String[] strArr;

		String pattern ="\\[[^\\d]*(\\d+)[^\\d]*\\]";
		Pattern p = Pattern.compile(pattern, Pattern.MULTILINE);
		Matcher _mnum=p.matcher(type);
		int blen=0;
		if(_mnum.find()) {
			String match = _mnum.group();
			match =match.replace("[", "");
			match = match.replace("]", "");
			blen= Integer.parseInt(match);
			
			strArr = new String[blen];
			type=type.substring(0,type.length()-(match.length()+2));
		}
		else{
			strArr= new String[10];
			type=type.substring(0,type.length()-2);
		}
		
		for(int i =0 ; i<strArr.length;i++) {
			StringBuilder buffer = new StringBuilder(targetStringLength);
		    for (int k = 0; k < targetStringLength; k++) {
		        int randomLimitedInt = hex_down + (int) 
		          (random.nextFloat() * (hex_up - hex_down + 1));
		        buffer.append((char) randomLimitedInt);
		    }
		    strArr[i] = buffer.toString();
		}
		
	    return strArr;
	}
	


}
