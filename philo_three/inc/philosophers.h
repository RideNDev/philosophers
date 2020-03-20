#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <fcntl.h>  
# include <semaphore.h>

# define MSG "msg_w"
# define FORK "fork_w"

typedef struct		s_philo
{
	int				name;
	int				last_eat;
	int				nb_of_eat;
	int				pid;
}					t_philo;

typedef struct		s_data
{
	t_philo			*philo;
	sem_t			*fork;
	sem_t			*msg;
	int				nb;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_time_each_philosophers_must_eat;
	int				philo_ok;
	int				time_start;
	int				philo_dead;
}					t_data;

extern		t_data *g_data;


//---------------------------------------------------------------------

int					init_game(int ac, char **av);
void        		init_philosophers(void);

int					start_game(void);
int         		end_game();

void				ft_philo(t_philo *philo);
void				*check_life(void *tmp_philo);
void				message(t_philo *philo, int msg);
int					get_time(void);

int					ft_atoi(const char *str);
void				ft_putnbr_fd(int nb, int fd);

//---------------------------------------------------------------------

#endif
