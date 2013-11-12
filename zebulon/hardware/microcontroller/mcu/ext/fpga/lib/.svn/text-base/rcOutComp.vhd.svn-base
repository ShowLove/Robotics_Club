-- R/C Signal Output Component
-- Robotics Club at UCF
-- Jonathan Mohlenhoff - jmohlenh@ist.ucf.edu
-- Stanislaus Bernard - sbernard@knights.ucf.edu

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD;

package rcOut is
	component rcOutComp is 
		Port
		(
			rcOut   : out std_logic;
			rcValue	: in std_logic_vector (7 downto 0);
			clock   : in std_logic
		);
	end component;
end package rcOut;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD;

entity rcOutComp is
	Port
	(
		rcOut   : out std_logic;
		rcValue	: in std_logic_vector (7 downto 0);
		clock   : in std_logic
	);
end rcOutComp;

architecture Behavioral of rcOutComp is

begin

process(clock)
variable counter : std_logic_vector (31 downto 0) := x"00000000";
variable period  : std_logic_vector (31 downto 0) := x"000C3500"; -- At 50 MHz base clock frequency, period will be 16ms
variable ontime  : std_logic_vector (31 downto 0);

begin	

	if (rising_edge(clock)) then 
		counter := counter + x"00000001";
		
		if(counter < ontime) then
			rcOut <= '1';
		elsif (counter < period) then 
			rcOut <= '0';
		else
			ontime := (rcValue * x"000000C3") + x"0000C350";
			counter := x"00000000";
		end if;
	end if;
	
end process;
	
end Behavioral;
