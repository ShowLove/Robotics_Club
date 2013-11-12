-- RC Signal Input Component
-- Robotics Club at UCF
-- Jonathan Mohlenhoff - jmohlenh@ist.ucf.edu
-- Stanislaus Bernard - sbernard@knight.ucf.edu

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD;

package rcIn is
	component rcInComp is 
		port
		(
			rcIn     : in std_logic;
			rcValue  : out std_logic_vector (7 downto 0);
			clock    : in std_logic
		);
	end component;
end package rcIn;


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD;

entity rcInComp is
	Port
	(
		rcIn     : in std_logic;
		rcValue  : out std_logic_vector (7 downto 0);
		clock    : in std_logic
	);
end rcInComp;

architecture Behavioral of rcInComp is

begin

process(clock)

variable countHigh, countLow : std_logic_vector (31 downto 0) := x"00000000";
variable temp    : std_logic_vector (31 downto 0);
variable temp2	  : std_logic_vector (8 downto 0);

begin
	if (rising_edge(clock)) then
		if (rcIn = '0') then
			if (countLow > x"0000C350") then
				if ((countHigh > x"0000C350") and (countHigh < x"000186A0")) then
					temp := countHigh * x"00000005";
					temp2 := temp (18 downto 10);
					temp2 := temp2 - 238;
					rcValue <= temp2 (7 downto 0);
				end if;
				countHigh := x"00000000";
			end if;
			countLow := countLow + x"00000001";
		else
			countHigh := countHigh + x"00000001";
			countLow := x"00000000";
		end if;
	end if;
end process;


end Behavioral;

