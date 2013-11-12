-- RC Signal Input Component
-- Robotics Club at UCF
-- Jonathan Mohlenhoff - jmohlenh@ist.ucf.edu
-- Stanislaus Bernard - sbernard@knight.ucf.edu

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD;

package rcInFBLR is
	component rcInCompFBLR is 
		port
		(
			rcIn     : in std_logic;
			rcValue  : out std_logic_vector (7 downto 0);
			clock    : in std_logic
		);
	end component;
end package rcInFBLR;


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD;

entity rcInCompFBLR is
	Port
	(
		rcIn     : in std_logic;
		rcValue  : out std_logic_vector (7 downto 0);
		clock    : in std_logic
	);
end rcInCompFBLR;

architecture Behavioral of rcInCompFBLR is

begin

process(clock)

variable countHigh : std_logic_vector (31 downto 0) := x"00000000";
variable countLow : std_logic_vector (31 downto 0) := x"00000000";
variable temp    : std_logic_vector (31 downto 0);
variable temp2	  : std_logic_vector (9 downto 0);

begin
	if (rising_edge(clock)) then
		if (rcIn = '0') then
			if (countLow > x"0000C350") then
				if ((countHigh > x"0000EA60") and (countHigh < x"00014C08")) then
					temp := countHigh * x"0000015";
					temp2 := temp (20 downto 11);
					temp2 := temp2 - 616;
					if (temp2 < "0000000000") then
						temp2 := "0000000000";
					end if;
					rcValue <= temp2 (7 downto 0);
				end if;
--				if (countHigh > x"00014C08") then 
--					temp2 := "0011111111";
--					rcValue <= temp2 (7 downto 0);
--				elsif (countHigh < x"0000EA60") then
--					temp2 := "0000000000";
--					rcValue <= temp2 (7 downto 0);
--				end if;
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

