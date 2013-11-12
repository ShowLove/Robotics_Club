library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

package PkToPk is
	component pkToPkComp is
	port (	
		clk : in std_logic;
		
		analog : in std_logic_vector (11 downto 0);
		maxValue : out std_logic_vector (11 downto 0);
		minValue : out std_logic_vector (11 downto 0);
		
		analogReady : in std_logic;
		decayValue : in std_logic_vector  (23 downto 0)
	);
	end component pkToPkComp;
end package PkToPk;

library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

entity pkToPkComp is
	port (
		clk : in std_logic;
		
		analog : in std_logic_vector (11 downto 0);
		maxValue : out std_logic_vector (11 downto 0);
		minValue : out std_logic_vector (11 downto 0);
		
		analogReady : in std_logic;
		decayValue : in std_logic_vector  (23 downto 0)
	);
end entity pkToPkComp;


architecture Behavioral of pkToPkComp is
	signal max : std_logic_vector (23 downto 0) := x"800000";
	signal min : std_logic_vector (23 downto 0) := x"800000";
begin

	process(clk)
		variable doneFlag : std_logic := '0';
		
	begin
		if (rising_edge(clk)) then
			if ((analogReady = '1') and (doneFlag = '0')) then
				if (max(23 downto 12) < analog) then
					max <= analog & x"FFF";
				elsif (max(23 downto 12) > analog) then
					max <= max - decayValue;
				end if;
				maxValue <= max (23 downto 12);
				
				if (min(23 downto 12) > analog) then
					min <= analog & x"000";
				elsif (min(23 downto 12) < analog) then
					min <= min + decayValue;
				end if;
				minValue <= min (23 downto 12);
				
				doneFlag := '1';
			elsif (analogReady = '0') then
				doneFlag := '0';				
			end if;
		end if;
	end process;

end architecture Behavioral;
