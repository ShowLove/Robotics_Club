-- 2 to 1 Mux Component
-- Robotics Club at UCF
-- Jonathan Mohlenhoff - jmohlenh@ist.ucf.edu

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package mux2to1 is
	component mux2to1Comp is
		Port
		(
			inputA    : in std_logic;
			inputB    : in std_logic;
			sel       : in std_logic;
			output    : out std_logic
		);
	end component;
end package mux2to1;
		

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity mux2to1Comp is
	Port
	(
		inputA    : in std_logic;
		inputB    : in std_logic;
		sel       : in std_logic;
		output    : out std_logic
	);
end mux2to1Comp;

architecture Behavioral of mux2to1Comp is

begin

process(inputA, inputB, sel)
begin

	case sel is
		when '0' =>
			output <= inputA;
		
		when '1' =>
			output <= inputB;
		
		when others =>
			--nothing
		
	end case;
	
end process;

end Behavioral;