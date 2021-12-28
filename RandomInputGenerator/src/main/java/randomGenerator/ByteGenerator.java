package randomGenerator;

import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ByteGenerator {
	
	private Random random = new Random();
	
	public byte[] byteGenerator(String _type)
	{
		byte[] bytes;
		Pattern p = Pattern.compile("\\d+");
		Matcher _mnum=p.matcher(_type);
		int blen=0;
		if(_mnum.find()) {
			blen=Integer.parseInt(_mnum.group());
			
			bytes = new byte[blen];
		}
		else{
			bytes= new byte[10];
		}
		try {
			
			random.nextBytes(bytes);
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return bytes;
	}
	
	public int[] byteGeneratorNew(String _type)
	{
		int[] ints;
	
		Pattern p = Pattern.compile("\\d+");
		Matcher _mnum=p.matcher(_type);
		int blen=0;
		if(_mnum.find()) {
			blen=Integer.parseInt(_mnum.group());
			ints = new int[blen];
		}
		else{
			ints= new int[10];
		}
		try {
			for(int i=0; i<ints.length;i++)
				ints[i]=random.nextInt(9)+1;
			
		}
		catch (Exception e) {
			throw e;
		}
		
		return ints;
	}
	
	public String[] byteGeneratorNewArray(String _type)
	{
		String[] bytes;
		String pattern ="\\[[^\\d]*(\\d+)[^\\d]*\\]";
		Pattern p = Pattern.compile(pattern,Pattern.MULTILINE);
		Matcher _mnum=p.matcher(_type);
		int blen=0;
		if(_mnum.find()) {
			String match = _mnum.group();
			match =match.replace("[", "");
			match = match.replace("]", "");
			blen= Integer.parseInt(match);
			bytes = new String[blen];
			_type=_type.substring(0,_type.length()-(match.length()+2));
			
			
		}
		else{
			bytes= new String[10];
		}
		try {
			
			for(int i=0;i<bytes.length;i++) {
				bytes[i] ="0x"+Integer.toString(random.nextInt(20)+10);
				
			}
		}
		catch (Exception e) {
			throw e;
		}
		
		return bytes;
	}
	

}
