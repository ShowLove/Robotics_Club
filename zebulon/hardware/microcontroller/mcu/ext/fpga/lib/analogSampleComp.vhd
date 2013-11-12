library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

package AnalogSample is
	component AnalogSampleComp is
	port (
		clk : in std_logic;
		
		sData : in std_logic_vector(5 downto 0);
		sClock : out std_logic_vector(2 downto 0);
		nCS : out std_logic_vector(2 downto 0);
	
		analog0 : out std_logic_vector (11 downto 0);
		analog1 : out std_logic_vector (11 downto 0);
		analog2 : out std_logic_vector (11 downto 0);
		analog3 : out std_logic_vector (11 downto 0);
		analog4 : out std_logic_vector (11 downto 0);
		analog5 : out std_logic_vector (11 downto 0);
		
		done : buffer std_logic
	);
	end component AnalogSampleComp;
end package AnalogSample;

library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;
use work.ADC.all;

entity AnalogSampleComp is
	port (
		clk : in std_logic;
		
		sData : in std_logic_vector(5 downto 0);
		sClock : out std_logic_vector(2 downto 0);
		nCS : out std_logic_vector(2 downto 0);
	
		analog0 : out std_logic_vector (11 downto 0);
		analog1 : out std_logic_vector (11 downto 0);
		analog2 : out std_logic_vector (11 downto 0);
		analog3 : out std_logic_vector (11 downto 0);
		analog4 : out std_logic_vector (11 downto 0);
		analog5 : out std_logic_vector (11 downto 0);
		
		done : buffer std_logic := '0'
	);
end entity AnalogSampleComp;


architecture Behavioral of AnalogSampleComp is
	
	signal reset : std_logic := '0';
	signal doneFlag : std_logic_vector(2 downto 0) := "000";
	signal start : std_logic := '1';
	signal tempAnalog0, tempAnalog1, tempAnalog2, tempAnalog3, tempAnalog4, tempAnalog5 : std_logic_vector(11 downto 0);
	
begin

	ad0: AD1RefComp port map
	(
	    CLK => clk,         
		 RST => reset,
		 SDATA1 => sData(0),
		 SDATA2 => sData(1),
		 SCLK => sClock(0),
		 nCS => nCS(0),

		 DATA1 => tempAnalog0,
		 DATA2 => tempAnalog1,
		 START => start, 
		 DONE => doneFlag(0)
	);
	
	ad1: AD1RefComp port map
	(
	    CLK => clk,         
		 RST => reset,
		 SDATA1 => sData(2),
		 SDATA2 => sData(3),
		 SCLK => sClock(1),
		 nCS => nCS(1),

		 DATA1 => tempAnalog2,
		 DATA2 => tempAnalog3,
		 START => start, 
		 DONE => doneFlag(1)
	);
	
	ad2: AD1RefComp port map
	(
	    CLK => clk,         
		 RST => reset,
		 SDATA1 => sData(4),
		 SDATA2 => sData(5),
		 SCLK => sClock(2),
		 nCS => nCS(2),

		 DATA1 => tempAnalog4,
		 DATA2 => tempAnalog5,
		 START => start, 
		 DONE => doneFlag(2)
	);

	process(clk, doneFlag)
	begin
		if (rising_edge(clk)) then
			if (doneFlag = "111") then
				analog0 <= tempAnalog0;
				analog1 <= tempAnalog1;
				analog2 <= tempAnalog2;
				analog3 <= tempAnalog3;
				analog4 <= tempAnalog4;
				analog5 <= tempAnalog5;
				start <= '1';
				done <= '1';
			elsif (doneFlag = "000") then
				start <= '0';
				done <= '0';
			end if;
		end if;
	end process;

end architecture Behavioral;
