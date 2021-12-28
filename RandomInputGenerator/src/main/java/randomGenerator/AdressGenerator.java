package randomGenerator;

import java.util.Random;


public class AdressGenerator {
	
	private String[] addresses = {"0xca35b7d915458ef540ade6068dfe2f44e8fa733c",
								  "0x14723a09acff6d2a60dcdf7aa4aff308fddc160c",
								  "0x4b0897b0513fdc7c541b6d9d7e929c4e5364d2db",
								  "0x583031d1113ad414f02576bd6afabfb302140225",
								  "0xdd870fa1b7c4700f2bd7f44238821c26f7392148"};
	
	
	public String addressGenerator(){
		
		
		int randomNum = Math.abs(new Random().nextInt()) % 5;
		
		return addresses[randomNum];
	}
	
	public String[] addressGeneratorList() {
		
		return addresses;
		
	}

}
