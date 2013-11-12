library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

package sampleFrequency is
	component sampleFrequencyComp is
	port (	
		clk : in std_logic;
		AdDone : in std_logic;
		
		AdFrequency : out std_logic_vector (31 downto 0); --Sampling frequency in Hz
		AdPeriod	: 	out std_logic_vector (11 downto 0) --Sampling period in nanoseconds
	);
	end component sampleFrequencyComp;
end package sampleFrequency;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

entity sampleFrequencyComp is
port(	
		clk : in std_logic;
		AdDone : in std_logic;
		
		AdFrequency : out std_logic_vector (31 downto 0); --Sampling frequency in Hz
		AdPeriod	: 	out std_logic_vector (11 downto 0) --Sampling period in nanoseconds
		);
end sampleFrequencyComp;

architecture Behavioral of sampleFrequencyComp is
begin

	process(clk, AdDone)
		type table is array (0 to 100) of std_logic_vector (31 downto 0);
		variable freqTable : table := ( x"00000000",
												x"02FAF080",
												x"017D7840",
												x"00FE502B",
												x"00BEBC20",
												x"00989680",
												x"007F2815",
												x"006CFDC9",
												x"005F5E10",
												x"0054C564",
												x"004C4B40",
												x"00455BAF",
												x"003F940B",
												x"003AB00A",
												x"00367EE5",
												x"0032DCD5",
												x"002FAF08",
												x"002CE0F8",
												x"002A62B2",
												x"0028279B",
												x"002625A0",
												x"00245498",
												x"0022ADD7",
												x"00212BD9",
												x"001FCA05",
												x"001E8480",
												x"001D5805",
												x"001C41CC",
												x"001B3F72",
												x"001A4EEA",
												x"00196E6B",
												x"00189C67",
												x"0017D784",
												x"00171E90",
												x"0016707C",
												x"0015CC5B",
												x"00153159",
												x"00149EB7",
												x"001413CD",
												x"00139003",
												x"001312D0",
												x"00129BB8",
												x"00122A4C",
												x"0011BE27",
												x"001156EC",
												x"0010F447",
												x"001095ED",
												x"00103B96",
												x"000FE503",
												x"000F91F8",
												x"000F4240",
												x"000EF5A8",
												x"000EAC02",
												x"000E6524",
												x"000E20E6",
												x"000DDF23",
												x"000D9FB9",
												x"000D6289",
												x"000D2775",
												x"000CEE62",
												x"000CB735",
												x"000C81D8",
												x"000C4E34",
												x"000C1C33",
												x"000BEBC2",
												x"000BBCCF",
												x"000B8F48",
												x"000B631D",
												x"000B383E",
												x"000B0E9E",
												x"000AE62E",
												x"000ABEE1",
												x"000A98AC",
												x"000A7384",
												x"000A4F5C",
												x"000A2C2B",
												x"000A09E7",
												x"0009E887",
												x"0009C802",
												x"0009A84F",
												x"00098968",
												x"00096B44",
												x"00094DDC",
												x"0009312A",
												x"00091526",
												x"0008F9CB",
												x"0008DF13",
												x"0008C4F9",
												x"0008AB76",
												x"00089286",
												x"00087A24",
												x"0008624B",
												x"00084AF6",
												x"00083422",
												x"00081DCB",
												x"000807EC",
												x"0007F281",
												x"0007DD88",
												x"0007C8FC",
												x"0007B4DB",
												x"0007A120"
											 );
	
		type table2 is array (0 to 15) of std_logic_vector (11 downto 0);
		variable movingAverageTable : table2 := ( x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000",
																x"000");
		variable doneCount : integer := 0;
		variable tempAverage : std_logic_vector (11 downto 0) := x"000";
		variable average : std_logic_vector (11 downto 0) := x"000";
		variable clkPeriod : std_logic_vector(11 downto 0) := x"000";
		variable doneFlag : std_logic := '0';
		
	begin 
		if(rising_edge(clk)) then
			clkPeriod := clkPeriod + x"001";
			
			if(AdDone = '1' and doneFlag = '0') then
				doneFlag := '1';
				movingAverageTable(doneCount) := clkPeriod;
				
				clkPeriod := x"000";
				
				doneCount := doneCount + 1;
				if (doneCount > 15) then
					doneCount := 0;
				else
				end if;
				
				tempAverage := movingAverageTable(0);
				tempAverage := tempAverage + movingAverageTable(1);
				tempAverage := tempAverage + movingAverageTable(2);
				tempAverage := tempAverage + movingAverageTable(3);
				tempAverage := tempAverage + movingAverageTable(4);
				tempAverage := tempAverage + movingAverageTable(5);
				tempAverage := tempAverage + movingAverageTable(6);
				tempAverage := tempAverage + movingAverageTable(7);
				tempAverage := tempAverage + movingAverageTable(8);
				tempAverage := tempAverage + movingAverageTable(9);
				tempAverage := tempAverage + movingAverageTable(10);
				tempAverage := tempAverage + movingAverageTable(11);
				tempAverage := tempAverage + movingAverageTable(12);
				tempAverage := tempAverage + movingAverageTable(13);
				tempAverage := tempAverage + movingAverageTable(14);
				tempAverage := tempAverage + movingAverageTable(15);
				
				average := x"0" & tempAverage (11 downto 4);
				
				AdPeriod <= average * x"014"; --Clock period is 20ns = 0x014 ns
				
				AdFrequency <= freqTable(conv_integer(unsigned(average)));
			
			elsif (AdDone = '0') then
				doneFlag := '0';
			
			end if;
		
		end if;
	
	end process;
			
end Behavioral;

