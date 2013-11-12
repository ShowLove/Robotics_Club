-- Square Wave Generator Component
-- Robotics Club at UCF
-- Jonathan Mohlenhoff - jmohlenh@ist.ucf.edu

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package squareWave is
	component squareWaveComp is
	port 
	(
		clock : in std_logic;
		freqOut	: out std_logic := '0';
		freqSelect : in std_logic_vector (7 downto 0)
	);
	end component squareWaveComp;
end package squareWave;


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity squareWaveComp is
PORT 
(
	clock : in std_logic;
	freqOut	: out std_logic := '0';
	freqSelect : in std_logic_vector (7 downto 0)
);
end squareWaveComp;

architecture Behavioral of squareWaveComp is 
begin

process(clock)
	variable temp : std_logic_vector(7 downto 0) := x"00";
	variable outValue : std_logic := '0';
	begin
		if (rising_edge(clock)) then
			if (temp = x"00") then
				temp := freqSelect;
				outValue := not outValue;
				freqOut <= outValue;
			else
				temp := temp - 1;
			end if;
		end if;
end process;

end Behavioral;	