#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct		s_philo
{
	int				name;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
	int				last_eat;
}					t_philo;

typedef struct		s_data
{
	t_philo			*philo;
	pthread_mutex_t	*fork;

	int				nb;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_time_each_philosophers_must_eat;
	int				time_start;
}					t_data;

extern		t_data *g_data;


//---------------------------------------------------------------------

int					init_game(int ac, char **av);
void        		init_philosophers(void);
int					start_game(void);	
int					get_time(void);
void				message(t_philo *philo, int msg);
void				*ft_life(void *philo);

int					ft_atoi(const char *str);
void				ft_putnbr_fd(int nb, int fd);

//---------------------------------------------------------------------

#endif
