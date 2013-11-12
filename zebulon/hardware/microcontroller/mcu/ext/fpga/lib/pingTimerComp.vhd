library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD;
use IEEE.std_logic_unsigned.all;

package PingTimer is
	component pingTimerComp is
	Port
	(
		clock		: in	std_logic;
		
		ping0		: in std_logic;
		ping1		: in std_logic;
		ping2		: in std_logic;
		ping3		: in std_logic;
		ping4		: in std_logic;

		pingTime0 : out std_logic_vector (31 downto 0) := x"00000000";
		pingTime1 : out std_logic_vector (31 downto 0) := x"00000000";
		pingTime2 : out std_logic_vector (31 downto 0) := x"00000000";
		pingTime3 : out std_logic_vector (31 downto 0) := x"00000000";
		pingTime4 : out std_logic_vector (31 downto 0) := x"00000000"
	);
	end component pingTimerComp;
end package PingTimer;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD;
use IEEE.std_logic_unsigned.all;
use work.RS232.all;

entity pingTimerComp is
	Port
	(
		clock		: in	std_logic;
		
		ping0		: in std_logic;
		ping1		: in std_logic;
		ping2		: in std_logic;
		ping3		: in std_logic;
		ping4		: in std_logic;

		pingTime0 : out std_logic_vector (31 downto 0) := x"00000000";
		pingTime1 : out std_logic_vector (31 downto 0) := x"00000000";
		pingTime2 : out std_logic_vector (31 downto 0) := x"00000000";
		pingTime3 : out std_logic_vector (31 downto 0) := x"00000000";
		pingTime4 : out std_logic_vector (31 downto 0) := x"00000000"
	);
end pingTimerComp;

architecture Behavioral of pingTimerComp is
	signal mPingTime0 : std_logic_vector (31 downto 0) := x"00000000";
	signal mPingTime1 : std_logic_vector (31 downto 0) := x"00000000";
	signal mPingTime2 : std_logic_vector (31 downto 0) := x"00000000";
	signal mPingTime3 : std_logic_vector (31 downto 0) := x"00000000";
	signal mPingTime4 : std_logic_vector (31 downto 0) := x"00000000";
	
	signal start : std_logic := '0';
	
begin
	
	process (clock)
	begin
		if (rising_edge(clock)) then
			if ((ping0 = '0') and (ping1 = '0') and (ping2 = '0') and (ping3 = '0') and (ping4 = '0')) then
				mPingTime0 <= x"00000000";
				mPingTime1 <= x"00000000";
				mPingTime2 <= x"00000000";
				mPingTime3 <= x"00000000";
				mPingTime4 <= x"00000000";
			
			elsif ((ping0 = '1') and (ping1 ='1') and (ping2 ='1') and (ping3 ='1') and (ping4 ='1')) then
				pingTime0 <= mPingTime0;
				pingTime1 <= mPingTime1;
				pingTime2 <= mPingTime2;
				pingTime3 <= mPingTime3;
				pingTime4 <= mPingTime4;
			
			else
				if (ping0 = '0') then
					mPingTime0 <= mPingTime0 + x"00000001";
				end if;
				
				if (ping1 = '0') then
					mPingTime1 <= mPingTime1 + x"00000001";
				end if;
				
				if (ping2 = '0') then
					mPingTime2 <= mPingTime2 + x"00000001";
				end if;
				
				if (ping3 = '0') then
					mPingTime3 <= mPingTime3 + x"00000001";
				end if;
				
				if (ping4 = '0') then
					mPingTime4 <= mPingTime4 + x"00000001";
				end if;
				
			end if;
			
		end if;
	end process;

end Behavioral;