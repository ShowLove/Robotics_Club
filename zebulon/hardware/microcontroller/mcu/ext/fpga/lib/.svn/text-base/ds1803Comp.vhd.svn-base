library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

package DS1803 is
	component ds1803Comp is
	port (
		clk : in std_logic;
		--nReset : in std_logic;

		--Dout : out std_logic_vector(7 downto 0);	-- data read from ds1803
		
		ChipSelect : in std_logic_vector(2 downto 0); -- Selects the chip on the bus at a particular address
		
		 -- Selects the POT in the chip
		 -- 00 = Not Allowed
		 -- 01 = POT0
		 -- 10 = POT1
		 -- 11 = Both POTS are set to same value
		POTSelect : in std_logic_vector(1 downto 0);
		
		DataIn : in std_logic_vector(7 downto 0);

		--error : out std_logic; -- no correct ack received

		SCL : inout std_logic;
		SDA : inout std_logic;
		
		setPot : in std_logic;
		potSet : out std_logic := '0'
	);
	end component ds1803Comp;
end package DS1803;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

use work.i2c.all;

entity ds1803Comp is
	port (
		clk : in std_logic;
		--nReset : in std_logic;

		--Dout : out std_logic_vector(7 downto 0);	-- data read from ds1803
		
		ChipSelect : in std_logic_vector(2 downto 0); -- Selects the chip on the bus at a particular address
		
		-- Selects the POT in the chip
		-- 00 = Not Allowed
		-- 01 = POT0
		-- 10 = POT1
		-- 11 = Both POTS are set to same value
		POTSelect : in std_logic_vector(1 downto 0);
		
		DataIn : in std_logic_vector(7 downto 0);

		--error : out std_logic; -- no correct ack received

		SCL : inout std_logic;
		SDA : inout std_logic;
		
		setPot : in std_logic;
		potSet : out std_logic := '0'
	);
end entity ds1803Comp;

architecture structural of ds1803Comp is
	constant SLAVE_ADDR : std_logic_vector(3 downto 0) := "0101";
	constant CLK_CNT : unsigned(7 downto 0) := conv_unsigned(25, 8);

	signal cmd_ack : std_logic;
	signal D : std_logic_vector(7 downto 0);
	signal lack, store_dout : std_logic;

	signal start, read, write, ack, stop : std_logic;
	signal i2c_dout : std_logic_vector(7 downto 0);
	
	signal nReset : std_logic := '0';
	
	signal Dout : std_logic_vector(7 downto 0);
	
	signal error : std_logic;
	
begin
	-- hookup I2C controller
	u1: i2cComponent port map (clk => clk, ena => '1', clk_cnt => clk_cnt, nReset => Not nReset,
			read => read, write => write, start => start, stop => stop, ack_in => ack, cmd_ack => cmd_ack, 
			Din => D, Dout => i2c_dout, ack_out => lack, SCL => SCL, SDA => SDA);
	
	init_statemachine : block
		type states is (waitForStart, s0, s1, s2, s3, done);
		signal state : states := waitForStart;
	begin
		nxt_state_decoder: process(clk, nReset, state)
			variable nxt_state : states;
			variable iD : std_logic_vector(7 downto 0);
			variable ierr : std_logic;
			variable istart, iread, iwrite, iack, istop : std_logic;
			variable istore_dout : std_logic;
			
		begin
			nxt_state := state;
			ierr := '0';
			istore_dout := '0';

			istart := start;
			iread := read;
			iwrite := write;
			iack := ack;
			istop := stop;
			iD := D;

			case (state) is
				when waitForStart =>
					if (setPot = '1') then
						nxt_state := s0;
					else
						potSet <= '0';
					end if;
					
				when s0 =>	-- send start condition, sent slave address + write
					nxt_state := s1;
					istart := '1';
					iread := '0';
					iwrite := '1';
					iack := '0';
					istop := '0';
					iD := (slave_addr & ChipSelect & '0'); -- write to slave (R/W = '0')

				when s1 =>	-- send "access config" command
					if (cmd_ack = '1') then
						nxt_state := s2;
						-- check aknowledge bit
						if (lack = '1') then
							ierr := '1'; -- no acknowledge received from last command, expected ACK
						end if;

						istart := '0';
						iread := '0';
						iwrite := '1';
						iack := '0';
						istop := '0';
						iD := "101010" & POTSelect;
					end if;

				when s2 =>	-- send config register data, sent stop condition
					if (cmd_ack = '1') then
						nxt_state := s3;
						-- check aknowledge bit
						if (lack = '1') then
							ierr := '1'; -- no acknowledge received from last command, expected ACK
						end if;

						istart := '0';
						iread := '0';
						iwrite := '1';
						iack := '0';
						istop := '1';
						iD := DataIn;
					end if;
					
				when s3 =>
					if (cmd_ack = '1') then
						potSet <= '1';
						iwrite := '0';
						nxt_state := done;
					end if;
					
				when done =>
					if (setPot = '0') then
						nxt_state := waitForStart;
					end if;
			end case;


			-- genregs
			if (nReset = '1') then
				state <= waitForStart;
				error <= '0';
				store_dout <= '0';

				start <= '0';
				read <= '0';
				write <= '0';
				ack <= '0';
				stop <= '0';
				D <= (others => '0');
			elsif (clk'event and clk = '1') then
				state <= nxt_state;
				error <= ierr;
				store_dout <= istore_dout;

				start <= istart;
				read <= iread;
				write <= iwrite;
				ack <= iack;
				stop <= istop;
				D <= iD;
			end if;
		end process nxt_state_decoder;
	end block init_statemachine;

	-- store temp
	gen_dout : process(clk)
	begin
		if (clk'event and clk = '1') then
			if (store_dout = '1') then
				Dout <= i2c_dout;
			end if;
		end if;
	end process gen_dout;

end architecture structural;


